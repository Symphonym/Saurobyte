#include "LuaEnv_Entity.hpp"
#include "Game.hpp"
#include "Systems/LuaSystem.hpp"
#include "Components/LuaComponent.hpp"

namespace jl
{
	int LuaEnv_Entity::GetComponent(lua_State* state)
	{
		// First arg is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		// Second argument is comp name
		std::string valueName = luaL_checkstring(state, 2);

		lua_settop(state, 0);
		BaseComponent *comp = entity->getComponent(valueName);
		if(comp == nullptr)
			lua_pushnil(state);
		else
			LuaEnvironment::pushObjectToLua<BaseComponent>(state, comp, "jl.Component");
		return 1;
	}
	int LuaEnv_Entity::GetComponentCount(lua_State* state)
	{
		// First arg is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		lua_pushnumber(state, entity->getComponentCount());
		return 1;
	}
	/*int LuaEnv_Entity::AddComponent(lua_State* state)
	{
		// First argument is entity to query, default is JL_ENTITY global
		lua_getglobal(state, "JL_ENTITY");
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "Entity");

		// Last argument is always the component to add
		BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, -1, "Component");
		entity->addComponent(comp->getTypeID(), comp);


		lua_settop(state, 0);

		return 0;
	}
	int LuaEnv_Entity::CreateComponent(lua_State* state)
	{
		// First argument is component name
		std::string valueName = luaL_checkstring(state, 1);
		lua_pop(state, 1);

		// Last argument is always the component to add
		//BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, -1, "Component");
		//entity->addComponent(comp->getTypeID(), comp);


		lua_settop(state, 0);

		return 1;
	}*/
	int LuaEnv_Entity::RemoveComponent(lua_State* state)
	{
		// First argument is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

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
	}
	int LuaEnv_Entity::HasComponent(lua_State* state)
	{
		// First argument is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		// Second argument is component name
		std::string valueName = luaL_checkstring(state, 2);

		lua_settop(state, 0);

		BaseComponent *comp = entity->getComponent(valueName);
		if(comp != nullptr)
			lua_pushboolean(state, 1);
		else
			lua_pushboolean(state, 0);

		return 1;
	}
	int LuaEnv_Entity::EnableEntity(lua_State* state)
	{
		// First argument is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		lua_settop(state, 0);

		entity->setActive(true);

		return 0;
	}
	int LuaEnv_Entity::DisableEntity(lua_State* state)
	{
		// First argument is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		lua_settop(state, 0);

		entity->setActive(false);

		return 0;
	}
	int LuaEnv_Entity::KillEntity(lua_State* state)
	{
		// First argument is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		lua_settop(state, 0);

		entity->kill();

		return 0;
	}
	int LuaEnv_Entity::GetID(lua_State *state)
	{
		// First arg is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		lua_pushnumber(state, entity->getID());
		return 1;
	}
	int LuaEnv_Entity::SubscribeEvent(lua_State *state)
	{
		// Lua system is upvalue
		LuaSystem* sys = static_cast<LuaSystem*>(lua_touserdata(state, lua_upvalueindex(1)));

		// First arg is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		// Second arg is event name
		std::string eventName = luaL_checkstring(state, 2);

		//LuaComponent *comp = entity->getComponent<LuaComponent>();
		//comp->requestEvent(eventName);
		sys->subscribeEntity(*entity, eventName);

		lua_settop(state, 0);

		return 0;
	}
	int LuaEnv_Entity::UnsubscribeEvent(lua_State *state)
	{
		// Lua system is upvalue
		LuaSystem* sys = static_cast<LuaSystem*>(lua_touserdata(state, lua_upvalueindex(1)));

		// First arg is self
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

		// Second arg is event name
		std::string eventName = luaL_checkstring(state, 2);

		//LuaComponent *comp = entity->getComponent<LuaComponent>();
		//comp->ignoreEvent(eventName);
		sys->unsubscribeEntity(*entity, eventName);

		lua_settop(state, 0);

		return 0;
	}


	void LuaEnv_Entity::exposeToLua(Game *game)
	{
		const luaL_Reg entityFuncs[] = 
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
		};
		game->getLua().registerClassToLua("jl.Entity", entityFuncs);

		// Event subscribing requires LuaSystem so we make them into C closures
		lua_State *state = game->getLua().getRaw();

		// Grab entity metatable
		luaL_getmetatable(state, "jl.Entity");
		int metaTable = lua_gettop(state);

		lua_pushlightuserdata(state, game->getSystemPool().getSystem<LuaSystem>());
		lua_pushcclosure(state, SubscribeEvent, 1);
		lua_setfield(state, metaTable, "SubscribeEvent");

		lua_pushlightuserdata(state, game->getSystemPool().getSystem<LuaSystem>());
		lua_pushcclosure(state, UnsubscribeEvent, 1);
		lua_setfield(state, metaTable, "UnsubscribeEvent");

		// Pop metatable value
		lua_pop(state, 1);

	}

};