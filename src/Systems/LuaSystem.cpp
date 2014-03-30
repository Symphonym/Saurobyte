#include "LuaSystem.hpp"
#include "Components/LuaComponent.hpp"
#include "Entity.hpp"
#include "Message.hpp"
#include "Game.hpp"

namespace jl
{


	LuaSystem::LuaSystem(Game *game)
		:
		System<LuaSystem>(game),
		m_luaEnv(game->getLua())
	{
		// Specify we want LuaComponents
		addRequirement({TypeIdGrabber::getUniqueTypeID<LuaComponent>()});

		// Subscribe to reload messages
		subscribe("ReloadLua");

		// Create lua state and environment
		//m_luaContext = luaL_newstate();
		//luaL_openlibs(m_luaContext);

		//createLuaEnvironment();

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
	}

	void LuaSystem::onMessage(jl::Message *message)
	{
		if(message->name == "ReloadLua")
		{
			for(auto itr = getEntities().begin(); itr != getEntities().end(); itr++)
			{
				LuaComponent *luaComp = itr->second->getComponent<LuaComponent>();
				runScript(*itr->second);
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
					lua_State *state = m_luaEnv.getRaw();

					// Grab event function
					lua_getglobal(state, "events");
					int eventFuncIndex = lua_gettop(state);

					// Push self
					LuaEnvironment::pushObjectToLua<Entity>(state, itr->second[i], "jl.Entity");

					// Push event name
					lua_pushstring(state, message->name.c_str());
					int argCount = 2;

					// Push event args, depending on event
					if(message->name == "KeyDown" || message->name == "KeyUp")
					{
						if(message->isType<SDL_Event>())
						{
							const SDL_Event& sdlEvent = static_cast<MessageData<SDL_Event>*>(message)->data;
							argCount += m_luaEnv.pushArgsToLua(
								LuaArg<std::string>{SDL_GetScancodeName(sdlEvent.key.keysym.scancode)},
								LuaArg<bool>{sdlEvent.key.repeat});
							
						}
					}

					else if(message->isType<std::string>())
					{
						std::string strData = static_cast<MessageData<std::string>*>(message)->data;
						lua_pushstring(state, strData.c_str());
						++argCount;
					}

					// Call event function
					if(!lua_isnil(state, eventFuncIndex))
						if(lua_pcall(state, argCount, 0, 0))
							m_luaEnv.reportError();
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

			lua_State *state = m_luaEnv.getRaw();

			// Call update function
			lua_getglobal(state, "update");

			// Push self
			LuaEnvironment::pushObjectToLua<Entity>(state, &entity, "jl.Entity");

			// Push delta time as argument TODO
			lua_pushnumber(state, game->getWindow().getDelta());

			if(!lua_isnil(state, -2))
			{
				if(lua_pcall(state, 2, 0, 0))
					m_luaEnv.reportError();
			}
		}

		// Only run Lua file if it's not loaded and has no errors, otherwise a reload is needed
		else if(!luaComp->runningStatus == LuaComponent::NotLoaded && !luaComp->runningStatus == LuaComponent::Errors)
			runScript(entity);
		
	}
	void LuaSystem::postProcess()
	{
	}
	void LuaSystem::onAttach(Entity &entity)
	{
		runScript(entity);
	}
	void LuaSystem::onDetach(Entity &entity)
	{
		// Iterate subscriptions
		LuaComponent *comp = entity.getComponent<LuaComponent>();
		for(auto eventItr = comp->subscribedEvents.begin(); eventItr != comp->subscribedEvents.end(); eventItr++)
		{
			auto itr = m_subscribedScripts.find(*eventItr);
			if(itr == m_subscribedScripts.end())
				continue;

			// Iterate entities associated with subscriptions
			for(std::size_t i = 0; i < itr->second.size(); i++)
			{
				// Unsubscribe entity if found
				if(itr->second[i] == &entity)
				{
					itr->second.erase(itr->second.begin() + i);

					// Unsubscribe from it entirely if none wants it, except the Reload event
					if(itr->first != "ReloadLua" && itr->second.empty())
					{
						unsubscribe(itr->first);
						m_subscribedScripts.erase(itr);
					}
					break;
				}
			}
		}
	}
	void LuaSystem::onKill(Entity &entity)
	{
		lua_State *state = m_luaEnv.getRaw();

		// Call kill function
		lua_getglobal(state, "killed");
		int killFuncIndex = lua_gettop(state);

		// Push self
		LuaEnvironment::pushObjectToLua<Entity>(state, &entity, "jl.Entity");

		if(!lua_isnil(state, killFuncIndex))
			if(lua_pcall(state, 1, 0, 0))
				m_luaEnv.reportError();
	}
	void LuaSystem::onClear()
	{		
		m_subscribedScripts.clear();
	}


	void LuaSystem::runScript(Entity &entity)
	{
		LuaComponent *luaComp = entity.getComponent<LuaComponent>();

		if(m_luaEnv.runScript(luaComp->luaFile.c_str()))
			luaComp->runningStatus = LuaComponent::Running;
		else
			luaComp->runningStatus = LuaComponent::Errors;

		lua_State *state = m_luaEnv.getRaw();

		// Call init function
		lua_getglobal(state, "init");
		int initFuncIndex = lua_gettop(state);

		// Push self
		LuaEnvironment::pushObjectToLua<Entity>(state, &entity, "jl.Entity");

		if(!lua_isnil(state, initFuncIndex))
			if(lua_pcall(state, 1, 0, 0))
				m_luaEnv.reportError();
	}


	void LuaSystem::subscribeEntity(Entity &entity, const std::string &eventName)
	{
		// Subscribe the LuaSystem to the specified event, and tell that the
		// entity that's related to this script is interested in such events.
		LuaComponent *comp = entity.getComponent<LuaComponent>();

		if(comp->subscribedEvents.find(eventName) == comp->subscribedEvents.end())
		{
			subscribe(eventName);
			comp->subscribedEvents.insert(eventName);
			m_subscribedScripts[eventName].push_back(&entity);
		}

	}
	void LuaSystem::unsubscribeEntity(Entity &entity, const std::string &eventName)
	{
		std::vector<Entity*>& scripts = m_subscribedScripts[eventName];
		for(std::size_t i = 0; i < scripts.size(); i++)
		{
			if(scripts[i] == &entity)
			{
				LuaComponent *comp = entity.getComponent<LuaComponent>();
				auto itr = comp->subscribedEvents.find(eventName);
				if(itr != comp->subscribedEvents.end())
					comp->subscribedEvents.erase(itr);

				scripts.erase(scripts.begin() + i);
				break;
			}
		}
	}




};