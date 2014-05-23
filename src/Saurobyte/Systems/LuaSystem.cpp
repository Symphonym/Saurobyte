
/*

The MIT License (MIT)

Copyright (c) 2014 by Jakob Larsson

Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <Saurobyte/Systems/LuaSystem.hpp>
#include <Saurobyte/Components/LuaComponent.hpp>
#include <Saurobyte/Entity.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/Game.hpp>
#include <Saurobyte/OpenGLWindow.hpp>

namespace Saurobyte
{


	LuaSystem::LuaSystem(Game *game)
		:
		System<LuaSystem>(game)
	{
		// Specify we want LuaComponents
		addRequirement({TypeIdGrabber::getUniqueTypeID<LuaComponent>()});

		// Subscribe to reload messages
		subscribe("ReloadLua");

		LuaEnvironment &env = game->getLua();

		env.pushObject(this, "Saurobyte_LuaSystem");
		env.writeGlobal("SAUROBYTE_LUA_SYSTEM");

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
		LuaEnvironment &env = game->getLua();
		env.pushNil();
		env.writeGlobal("SAUROBYTE_LUA_SYSTEM");
	}

	void LuaSystem::onMessage(Message *message)
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
					/*lua_State *state = m_luaEnv.getRaw();

					// Grab event function
					lua_getglobal(state, "events");
					int eventFuncIndex = lua_gettop(state);

					// Push self
					LuaEnvironment::pushObject<Entity>(state, itr->second[i], "jl.Entity");

					// Push event name
					lua_pushstring(state, message->name.c_str());
					int argCount = 2;

					// Push event args, depending on event
					if(message->name == "KeyDown" || message->name == "KeyUp")
					{
						if(message->isType<SDL_Event>())
						{
							const SDL_Event& sdlEvent = static_cast<MessageData<SDL_Event>*>(message)->data;
							argCount += m_luaEnv.pushArgs(
								std::string(SDL_GetScancodeName(sdlEvent.key.keysym.scancode)),
								static_cast<bool>(sdlEvent.key.repeat));
							
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
							m_luaEnv.reportError();*/
				}
			}
		}

		
	}

	void LuaSystem::processEntity(Entity &entity)
	{
		LuaComponent *luaComp = entity.getComponent<LuaComponent>();

		LuaEnvironment &env = game->getLua();

		//env.pushArgs(game->getWindow().getDelta());
		env.callFunction("entity.update", 1, luaComp->sandBox);

		// Call lua script as usual
		if(luaComp->runningStatus == LuaComponent::Running)
		{

			/*lua_State *state = m_luaEnv.getRaw();

			// Call update function
			lua_getglobal(state, "update");

			// Push self
			LuaEnvironment::pushObject<Entity>(state, &entity, "jl.Entity");

			// Push delta time as argument TODO
			// TODO lua_pushnumber(state, game->getWindow().getDelta());

			if(!lua_isnil(state, -2))
			{
				if(lua_pcall(state, 2, 0, 0))
					m_luaEnv.reportError();
			}*/
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
		LuaEnvironment &env = game->getLua();

		LuaComponent *luaComp = entity.getComponent<LuaComponent>();
		luaComp->sandBox = env.createSandbox(
			{
				"os.execute"
			});

		env.pushObject<Entity*>(&entity, "Saurobyte_Entity");
		env.writeGlobal("entity", luaComp->sandBox);

		env.runScript(luaComp->luaFile, luaComp->sandBox);
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
		/*lua_State *state = m_luaEnv.getRaw();

		// Call kill function
		lua_getglobal(state, "killed");
		int killFuncIndex = lua_gettop(state);

		// Push self
		LuaEnvironment::pushObject<Entity>(state, &entity, "jl.Entity");

		if(!lua_isnil(state, killFuncIndex))
			if(lua_pcall(state, 1, 0, 0))
				m_luaEnv.reportError();*/
	}
	void LuaSystem::onClear()
	{		
		m_subscribedScripts.clear();
	}


	void LuaSystem::runScript(Entity &entity)
	{
		/*LuaComponent *luaComp = entity.getComponent<LuaComponent>();

		if(m_luaEnv.runScript(luaComp->luaFile.c_str()))
			luaComp->runningStatus = LuaComponent::Running;
		else
			luaComp->runningStatus = LuaComponent::Errors;

		lua_State *state = m_luaEnv.getRaw();

		// Call init function
		lua_getglobal(state, "init");
		int initFuncIndex = lua_gettop(state);

		// Push self
		LuaEnvironment::pushObject<Entity>(state, &entity, "jl.Entity");

		if(!lua_isnil(state, initFuncIndex))
			if(lua_pcall(state, 1, 0, 0))
				m_luaEnv.reportError();*/
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