#include "LuaSystem.hpp"
#include "Components/LuaComponent.hpp"
#include "Entity.hpp"
#include "Message.hpp"
#include "Game.hpp"
#include "Logger.hpp"

namespace jl
{


	LuaSystem::LuaSystem(Game *game)
		:
		System<LuaSystem>(game)
	{
		// Specify we want LuaComponents
		addRequirement({TypeIdGrabber::getUniqueTypeID<LuaComponent>()});

		// Subscribe to reload messages
		subscribe("ReloadLua");

		// Create lua state and environment
		m_luaContext = luaL_newstate();
		luaL_openlibs(m_luaContext);

		createLuaEnvironment();

		//lua_pushstring(m_luaContext, "BRAT");
		//lua_pushnumber(m_luaContext, 1337);
		//lua_pushnumber(m_luaContext, 55);
		//lua_pushnumber(m_luaContext, 3333);

		//reverse_variadic<4, Tester> testerRec;
		//Tester* test = 
		//testerRec.create(m_luaContext);
		//SDL_Log("TEST %i %i %i %c", test->numA, test->numB, test->numC, test->strA);
	}
	LuaSystem::~LuaSystem()
	{
		lua_close(m_luaContext);
	}

	void LuaSystem::onMessage(jl::Message *message)
	{
		if(message->name == "ReloadLua")
		{
			for(auto itr = getEntities().begin(); itr != getEntities().end(); itr++)
			{
				LuaComponent *luaComp = itr->second->getComponent<LuaComponent>();
				runLuaFile(*itr->second);
			}
		}
		else
		{
			// Find entities (scripts) associated with this message
			auto itr = m_subscribedScripts.find(message->name);

			if(itr != m_subscribedScripts.end())
			{
				for(std::size_t i = 0; i < itr->second.size(); i++)
				{
					// Grab event function
					lua_getglobal(m_luaContext, "events");
					int eventFuncIndex = lua_gettop(m_luaContext);

					// Push self
					pushObjectToLua<Entity>(m_luaContext, itr->second[i], "jl.Entity");

					// Push event name
					lua_pushstring(m_luaContext, message->name.c_str());
					int argCount = 2;

					// Push event args, depending on event
					if(message->name == "KeyDown" || message->name == "KeyUp")
					{
						if(message->isType<SDL_Event>())
						{
							const SDL_Event& sdlEvent = static_cast<MessageData<SDL_Event>*>(message)->data;
							argCount += pushArgsToLua(
								LuaArg<std::string>{SDL_GetScancodeName(sdlEvent.key.keysym.scancode)},
								LuaArg<bool>{sdlEvent.key.repeat});
							
						}
					}

					// Call event function
					if(!lua_isnil(m_luaContext, eventFuncIndex))
						if(lua_pcall(m_luaContext, argCount, 0, 0))
							reportLuaError(*itr->second[i], -1);
				}
			}
		}

		
	}

	void LuaSystem::processEntity(Entity &entity)
	{
		LuaComponent *luaComp = entity.getComponent<LuaComponent>();

		// Call lua script as usual
		if(luaComp->runningStatus == LuaComponent::Running)
		{
			// Call update function
			lua_getglobal(m_luaContext, "update");

			// Push self
			pushObjectToLua<Entity>(m_luaContext, &entity, "jl.Entity");

			// Push delta time as argument TODO
			lua_pushnumber(m_luaContext, game->getWindow().getDelta());

			if(!lua_isnil(m_luaContext, -2))
			{
				if(lua_pcall(m_luaContext, 2, 0, 0))
					reportLuaError(entity, -1);;
			}
		}

		// Only run Lua file if it's not loaded and has no errors, otherwise a reload is needed
		else if(!luaComp->runningStatus == LuaComponent::NotLoaded && !luaComp->runningStatus == LuaComponent::Errors)
			runLuaFile(entity);
		
	}
	void LuaSystem::postProcess()
	{
	}
	void LuaSystem::onEntityAdded(Entity &entity)
	{
		runLuaFile(entity);
	}
	void LuaSystem::onEntityRemoved(Entity &entity)
	{
		// Iterate subscriptions
		for(auto itr = m_subscribedScripts.begin(); itr != m_subscribedScripts.end(); itr++)
		{
			// Iterate entities associated with subscriptions
			for(std::size_t i = 0; i < itr->second.size(); i++)
			{
				// Unsubscribe entity if found
				if(itr->second[i] == &entity)
				{
					itr->second.erase(itr->second.begin() + i);

					// Unsubscribe from it entirely if none wants it
					if(itr->first != "ReloadLua" && itr->second.empty())
						unsubscribe(itr->first);
					return;
				}
			}
		}
	}
	void LuaSystem::onSystemCleared()
	{
		m_subscribedScripts.clear();
	}

	std::size_t LuaSystem::getMemoryUsage() const
	{
		return lua_gc(m_luaContext, LUA_GCCOUNT, 0);
	}


	void LuaSystem::runLuaFile(Entity &entity)
	{
		LuaComponent *luaComp = entity.getComponent<LuaComponent>();

		if(luaL_dofile(m_luaContext, luaComp->luaFile.c_str()))
			reportLuaError(entity, -1);
		else
			luaComp->runningStatus = LuaComponent::Running;

		// Call init function
		lua_getglobal(m_luaContext, "init");
		int initFuncIndex = lua_gettop(m_luaContext);

		// Push self
		pushObjectToLua<Entity>(m_luaContext, &entity, "jl.Entity");

		if(!lua_isnil(m_luaContext, initFuncIndex))
			if(lua_pcall(m_luaContext, 1, 0, 0))
				reportLuaError(entity, -1);
	}

	void LuaSystem::reportLuaError(Entity &entity, int errorIndex)
	{
		LuaComponent *luaComp = entity.getComponent<LuaComponent>();
		luaComp->runningStatus = LuaComponent::Errors;

		JL_ERROR_LOG("LUA ERROR: %s", lua_tostring(m_luaContext, errorIndex));
	}

	void LuaSystem::registerClassToLua(const std::string &className, const luaL_Reg *funcs)
	{
		luaL_newmetatable(m_luaContext, className.c_str());
		int metaTable = lua_gettop(m_luaContext);

		luaL_setfuncs(m_luaContext, funcs, 0);

		lua_pushvalue(m_luaContext, -1);
		lua_setfield(m_luaContext, metaTable, "__index");
	}

	void LuaSystem::subscribeEntity(Entity &entity, const std::string &eventName)
	{
		// Subscribe the LuaSystem to the specified event, and tell that the
		// entity that's related to this script is interested in such events.
		subscribe(eventName);
		std::vector<Entity*>& scripts = m_subscribedScripts[eventName];

		// Make sure we're not already subscribed
		bool notSubscribed = true;
		for(std::size_t i = 0; i < scripts.size(); i++)
		{
			if(scripts[i] == &entity)
			{
				notSubscribed = false;
				break;
			}
		}

		if(notSubscribed)
			scripts.push_back(&entity);
	}
	void LuaSystem::unsubscribeEntity(Entity &entity, const std::string &eventName)
	{
		std::vector<Entity*>& scripts = m_subscribedScripts[eventName];
		for(std::size_t i = 0; i < scripts.size(); i++)
		{
			if(scripts[i] == &entity)
			{
				scripts.erase(scripts.begin() + i);
				break;
			}
		}
	}








	void LuaSystem::createLuaEnvironment()
	{
		// Define all functions as lambdas

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//            Entity specific functions
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Getter for components themselves
		auto GetComponent = [] (lua_State* state) -> int
		{
			// First arg is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			// Second argument is comp name
			std::string valueName = luaL_checkstring(state, 2);

			lua_settop(state, 0);
			BaseComponent *comp = entity->getComponent(valueName);
			if(comp == nullptr)
				lua_pushnil(state);
			else
				pushObjectToLua<BaseComponent>(state, comp, "jl.Component");
			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Get component count of entity
		auto GetComponentCount = [] (lua_State* state) -> int
		{
			// First arg is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			lua_pushnumber(state, entity->getComponentCount());
			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Add component to entity
		auto AddComponent = [] (lua_State* state) -> int
		{
			// First argument is entity to query, default is JL_ENTITY global
			lua_getglobal(state, "JL_ENTITY");
			Entity* entity = convertUserdata<Entity>(state, 1, "Entity");

			// Last argument is always the component to add
			BaseComponent *comp = convertUserdata<BaseComponent>(state, -1, "Component");
			entity->addComponent(comp->getTypeID(), comp);


			lua_settop(state, 0);

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Create a component TODO with creatinf and add n shit
		auto CreateComponent = [] (lua_State* state) -> int
		{
			// First argument is component name
			std::string valueName = luaL_checkstring(state, 1);
			lua_pop(state, 1);

			// Last argument is always the component to add
			//BaseComponent *comp = convertUserdata<BaseComponent>(state, -1, "Component");
			//entity->addComponent(comp->getTypeID(), comp);


			lua_settop(state, 0);

			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Remove components
		auto RemoveComponent = [] (lua_State* state) -> int
		{
			// First argument is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			// Second argument is component name
			std::string valueName = luaL_checkstring(state, 2);

			lua_settop(state, 0);

			BaseComponent *comp = entity->getComponent(valueName);
			if(comp != nullptr)
			{
				entity->removeComponent(comp->getTypeID());
				lua_pushboolean(state, 1);
			}
			else
				lua_pushboolean(state, 0);

			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Has component
		auto HasComponent = [] (lua_State* state) -> int
		{
			// First argument is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			// Second argument is component name
			std::string valueName = luaL_checkstring(state, 2);

			lua_settop(state, 0);

			BaseComponent *comp = entity->getComponent(valueName);
			if(comp != nullptr)
				lua_pushboolean(state, 1);
			else
				lua_pushboolean(state, 0);

			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Enable entity
		auto EnableEntity = [] (lua_State* state) -> int
		{
			// First argument is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			lua_settop(state, 0);

			entity->setActive(true);

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Disable entity
		auto DisableEntity = [] (lua_State* state) -> int
		{
			// First argument is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			lua_settop(state, 0);

			entity->setActive(false);

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Kill entity
		auto KillEntity = [] (lua_State* state) -> int
		{
			// First argument is self
			Entity* entity = convertUserdata<Entity>(state, 1, "jl.Entity");

			lua_settop(state, 0);

			entity->kill();

			return 0;
		};


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//            Component specific functions
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Getter for component values
		auto GetComponentValue = [] (lua_State* state) -> int
		{
			// First argument is self
			BaseComponent *comp = convertUserdata<BaseComponent>(state, 1, "jl.Component");

			// Second argument is value identifier
			std::string valueName = luaL_checkstring(state, 2);

			return comp->onLuaGet(valueName, state);
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Setter for component values
		auto SetComponentValue = [] (lua_State* state) -> int
		{
			// First argument is self
			BaseComponent *comp = convertUserdata<BaseComponent>(state, 1, "jl.Component");

			// Second argument is value identifier
			std::string valueName = luaL_checkstring(state, 2);

			// Third values and so forth are optional arguments handled in
			// the component. But we will remove first and second args
			// to make it easier on the users end.
			lua_remove(state, 1); // Pop two front elements
			lua_remove(state, 1);

			comp->onLuaSet(valueName, state);
			return 0;
		};

		// Get component name
		auto GetComponentName = [] (lua_State* state) -> int
		{
			// First argument is self
			BaseComponent *comp = convertUserdata<BaseComponent>(state, 1, "jl.Component");

			lua_pushstring(state, comp->getName().c_str());
			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//            Game specific functions
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Change scene
		auto ChangeScene = [] (lua_State* state) -> int
		{
			// First arg is self
			Game* game = convertUserdata<Game>(state, 1, "jl.Game");

			// Second argument is scene name
			std::string sceneName = luaL_checkstring(state, 2);

			lua_settop(state, 0);

			game->changeScene(sceneName.c_str());

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Delete scene
		auto DeleteScene = [] (lua_State* state) -> int
		{
			// First arg is self
			Game* game = convertUserdata<Game>(state, 1, "jl.Game");

			// Second argument is scene name
			std::string sceneName = luaL_checkstring(state, 2);

			lua_settop(state, 0);

			game->getScenePool().deleteScene(sceneName.c_str());

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Get total entity count
		auto GetTotalEntityCount = [] (lua_State* state) -> int
		{
			// First arg is self
			Game* game = convertUserdata<Game>(state, 1, "jl.Game");

			lua_settop(state, 0);

			lua_pushnumber(state, game->getEntityPool().getEntityCount());

			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Move scene camera
		auto MoveCamera = [] (lua_State* state) -> int
		{
			// First arg is self
			Game* game = convertUserdata<Game>(state, 1, "jl.Game");

			// 2nd arg is X offset, 3rd arg is Y offset, 4th arg is Z offset
			float xOff = luaL_checknumber(state, 2);
			float yOff = luaL_checknumber(state, 3);
			float zOff = luaL_checknumber(state, 4);

			lua_settop(state, 0);

			game->getScenePool().getActiveScene()->getCamera().move(glm::vec3(xOff, yOff, zOff));

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//            System specific functions
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Subscribe to events
		auto SubscribeEvent = [] (lua_State* state) -> int
		{
			// First arg is self
			LuaSystem* sys = convertUserdata<LuaSystem>(state, 1, "jl.LuaSystem");

			// Second arg is entity to subscribe to the event
			Entity* entity = convertUserdata<Entity>(state, 2, "jl.Entity");

			// First argument is event name
			std::string eventName = luaL_checkstring(state, 3);

			// Subscribe the LuaSystem to the specified event, and tell that the
			// entity that's related to this script is interested in such events.
			sys->subscribe(eventName);
			std::vector<Entity*>& scripts = sys->m_subscribedScripts[eventName];

			// Make sure we're not already subscribed
			bool notSubscribed = true;
			for(std::size_t i = 0; i < scripts.size(); i++)
			{
				if(scripts[i] == entity)
				{
					notSubscribed = false;
					break;
				}
			}

			if(notSubscribed)
				scripts.push_back(entity);

			lua_settop(state, 0);

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Unsubscribe to events
		auto UnsubscribeEvent = [] (lua_State* state) -> int
		{
			// First arg is self
			LuaSystem* sys = convertUserdata<LuaSystem>(state, 1, "jl.LuaSystem");

			// Second arg is entity to subscribe to the event
			Entity* entity = convertUserdata<Entity>(state, 2, "jl.Entity");

			// First argument is event name
			std::string eventName = luaL_checkstring(state, 3);

			std::vector<Entity*>& scripts = sys->m_subscribedScripts[eventName];
			for(std::size_t i = 0; i < scripts.size(); i++)
			{
				if(scripts[i] == entity)
				{
					scripts.erase(scripts.begin() + i);
					break;
				}
			}

			lua_settop(state, 0);

			return 0;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//            Input specific functions
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Get mouse position
		auto GetMousePos = [] (lua_State* state) -> int
		{
			int mouseX = -1, mouseY = -1;
			SDL_GetMouseState(&mouseX, &mouseY);

			lua_settop(state, 0);

			lua_pushnumber(state, mouseX);
			lua_pushnumber(state, mouseY);
			return 2;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Mouse pressed
		auto IsMousePressed = [] (lua_State* state) -> int
		{
			// First argument is mouse key
			std::string mouseKeyName = luaL_checkstring(state, 1);

			lua_settop(state, 0);

			Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
			if(mouseKeyName == "left")
				lua_pushboolean(state, mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
			else if(mouseKeyName == "right")
				lua_pushboolean(state, mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));
			else if(mouseKeyName == "middle")
				lua_pushboolean(state, mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE));


			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Keyboard key pressed
		auto IsKeyPressed = [] (lua_State* state) -> int
		{
			// First argument is keyboard key
			std::string keyboardKeyName = luaL_checkstring(state, 1);

			lua_settop(state, 0);

			SDL_Keycode keyCode = SDL_GetKeyFromName(keyboardKeyName.c_str());

			if(keyCode == SDLK_UNKNOWN)
				lua_pushboolean(state, 0);
			else
			{
				const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
				lua_pushboolean(state, keyboardState[SDL_GetScancodeFromKey(keyCode)]);
			}

			return 1;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		const luaL_Reg entityFuncs[] = 
		{
			{ "GetComponent", GetComponent },
			{ "GetComponentCount", GetComponentCount },
			{ "RemoveComponent", RemoveComponent },
			{ "HasComponent", HasComponent },
			{ "EnableEntity", EnableEntity },
			{ "DisableEntity", DisableEntity },
			{ "KillEntity", KillEntity },
			{ NULL, NULL }
		};
		registerClassToLua("jl.Entity", entityFuncs);

		const luaL_Reg componentFuncs[] = 
		{
			{ "GetValue", GetComponentValue },
			{ "SetValue", SetComponentValue },
			{ "GetName", GetComponentName },
			{ NULL, NULL }
		};
		registerClassToLua("jl.Component", componentFuncs);
		
		const luaL_Reg systemFuncs[] = 
		{
			{ "SubscribeEvent", SubscribeEvent },
			{ "UnsubscribeEvent", UnsubscribeEvent },
			{ NULL, NULL }
		};
		registerClassToLua("jl.LuaSystem", systemFuncs);

		
		const luaL_Reg gameFuncs[] = 
		{
			{ "ChangeScene", ChangeScene },
			{ "DeleteScene", DeleteScene },
			{ "GetTotalEntityCount", GetTotalEntityCount },
			{ "MoveCamera", MoveCamera },
			{ NULL, NULL }
		};
		registerClassToLua("jl.Game", gameFuncs);
		const luaL_Reg inputFuncs[] = 
		{
			{ "GetMousePos", GetMousePos },
			{ "IsMousePressed", IsMousePressed },
			{ "IsKeyPressed", IsKeyPressed },
			{ NULL, NULL }
		};
		lua_pushglobaltable(m_luaContext);
		luaL_setfuncs(m_luaContext, inputFuncs, 0);


		// Setup default lua entity environment
		pushObjectToLua<Game>(m_luaContext, game, "jl.Game");
		lua_setglobal(m_luaContext, "game");

		pushObjectToLua<LuaSystem>(m_luaContext, this, "jl.LuaSystem");
		lua_setglobal(m_luaContext, "system");
	}
};