
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


#include <Saurobyte/Lua/LuaEnv_Entity.hpp>
#include <Saurobyte/Systems/LuaSystem.hpp>
#include <Saurobyte/Components/LuaComponent.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Game.hpp>
#include <Saurobyte/Logger.hpp>


namespace Saurobyte
{
	int LuaEnv_Entity::GetComponent(LuaEnvironment &env)
	{
		// First arg is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");


		// Second argument is comp name
		std::string valueName = env.readArg<std::string>();

		SAUROBYTE_INFO_LOG("GETTERU %s", valueName.c_str());

		BaseComponent *comp = entity->getComponent(valueName);
		if(comp == nullptr)
			env.pushNil();
		else
			env.pushObject<BaseComponent*>(comp, "Saurobyte_Component");
		return 1;
	}
	int LuaEnv_Entity::GetComponentCount(LuaEnvironment &env)
	{
		// First arg is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		env.pushArgs(entity->getComponentCount());
		return 1;
	}
	/*int LuaEnv_Entity::AddComponent(LuaEnvironment &env)
	{
		// First argument is entity to query, default is JL_ENTITY global
		lua_getglobal(state, "JL_ENTITY");
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "Entity");

		// Last argument is always the component to add
		BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, -1, "Component");
		entity->addComponent(comp->typeID, comp);


		lua_settop(state, 0);

		return 0;
	}
	int LuaEnv_Entity::CreateComponent(LuaEnvironment &env)
	{
		// First argument is component name
		std::string valueName = luaL_checkstring(state, 1);
		lua_pop(state, 1);

		// Last argument is always the component to add
		//BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, -1, "Component");
		//entity->addComponent(comp->typeID, comp);


		lua_settop(state, 0);

		return 1;
	}*/
	int LuaEnv_Entity::RemoveComponent(LuaEnvironment &env)
	{
		// First argument is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		// Second argument is component name
		std::string valueName = env.readArg<std::string>();

		BaseComponent *comp = entity->getComponent(valueName);
		if(comp != nullptr)
		{
			entity->removeComponent(comp->getTypeID());
			env.pushArgs(true);
		}
		else
			env.pushArgs(false);

		return 1;
	}
	int LuaEnv_Entity::HasComponent(LuaEnvironment &env)
	{
		// First argument is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		// Second argument is component name
		std::string valueName = env.readArg<std::string>();

		BaseComponent *comp = entity->getComponent(valueName);
		if(comp != nullptr)
			env.pushArgs(true);
		else
			env.pushArgs(false);

		return 1;
	}
	int LuaEnv_Entity::EnableEntity(LuaEnvironment &env)
	{
		// First argument is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		entity->setActive(true);

		return 0;
	}
	int LuaEnv_Entity::DisableEntity(LuaEnvironment &env)
	{
		// First argument is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		entity->setActive(false);

		return 0;
	}
	int LuaEnv_Entity::KillEntity(LuaEnvironment &env)
	{
		// First argument is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		entity->kill();

		return 0;
	}
	int LuaEnv_Entity::GetID(LuaEnvironment &env)
	{
		// First arg is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		env.pushArgs(entity->getID());
		return 1;
	}
	int LuaEnv_Entity::SubscribeEvent(LuaEnvironment &env)
	{
		// Lua system is upvalue
		//LuaSystem* sys = static_cast<LuaSystem*>(lua_touserdata(state, lua_upvalueindex(1)));

		// First arg is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		// Second arg is event name
		std::string eventName = env.readArg<std::string>();

		if(env.readGlobal("SAUROBYTE_LUA_SYSTEM"))
		{
			LuaSystem *sys = env.readArg<LuaSystem*>();
			sys->subscribeEntity(*entity, eventName);
		}

		//LuaComponent *comp = entity->getComponent<LuaComponent>();
		//comp->requestEvent(eventName);
		//sys->subscribeEntity(*entity, eventName);

		return 0;
	}
	int LuaEnv_Entity::UnsubscribeEvent(LuaEnvironment &env)
	{
		// Lua system is upvalue
		//LuaSystem* sys = static_cast<LuaSystem*>(lua_touserdata(state, lua_upvalueindex(1)));

		// First arg is self
		Entity *entity = env.readArg<Entity*>("Saurobyte_Entity");

		// Second arg is event name
		std::string eventName = env.readArg<std::string>();

		if(env.readGlobal("SAUROBYTE_LUA_SYSTEM"))
		{
			LuaSystem *sys = env.readArg<LuaSystem*>();
			sys->unsubscribeEntity(*entity, eventName);
		}

		//LuaComponent *comp = entity->getComponent<LuaComponent>();
		//comp->ignoreEvent(eventName);
		//sys->unsubscribeEntity(*entity, eventName);

		return 0;
	}


	void LuaEnv_Entity::exposeToLua(Game *game)
	{
		/*const luaL_Reg entityFuncs[] = 
		{
			{ "GetComponent", GetComponent },
			{ "GetComponentCount", GetComponentCount },
			{ "RemoveComponent", RemoveComponent },
			{ "HasComponent", HasComponent },
			{ "Enable", EnableEntity },
			{ "Disable", DisableEntity },
			{ "Kill", KillEntity },
			{ "GetID", GetID },
			{ NULL, NULL }
		};*/
		//game->getLua().createClass("Saurobyte_Entity", entityFuncs);

		LuaEnvironment &env = game->getLua();

		env.pushArgs(game->getSystemPool().getSystem<LuaSystem>());
		env.writeGlobal("SAUROBYTE_LUA_SYSTEM");

		env.createClass("Saurobyte_Entity",
		{
			{ "GetComponent", GetComponent },
			{ "GetComponentCount", GetComponentCount },
			{ "RemoveComponent", RemoveComponent },
			{ "HasComponent", HasComponent },
			{ "Enable", EnableEntity },
			{ "Disable", DisableEntity },
			{ "Kill", KillEntity },
			{ "GetID", GetID }
		});

		// Event subscribing requires LuaSystem so we make them into C closures
	//	LuaEnvironment &env = game->getLua().getRaw();

		// Grab entity metatable
		//luaL_getmetatable(state, "Saurobyte_Entity");
		//int metaTable = lua_gettop(state);

		//lua_pushlightuserdata(state, game->getSystemPool().getSystem<LuaSystem>());
		//lua_pushcclosure(state, SubscribeEvent, 1);
		//lua_setfield(state, metaTable, "SubscribeEvent");

	//	lua_pushlightuserdata(state, game->getSystemPool().getSystem<LuaSystem>());
		//lua_pushcclosure(state, UnsubscribeEvent, 1);
		//lua_setfield(state, metaTable, "UnsubscribeEvent");

		// Pop metatable value
		//lua_pop(state, 1);

	}

};