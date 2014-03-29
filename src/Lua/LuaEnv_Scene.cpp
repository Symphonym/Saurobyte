#include "LuaEnv_Scene.hpp"
#include "Game.hpp"

namespace jl
{
	int LuaEnv_Scene::GetEntities(lua_State *state)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		lua_createtable(state, scene->getEntities().size(), 0);
		int tableIndex = lua_gettop(state);

		int index = 1;
		for(auto itr = scene->getEntities().begin(); itr != scene->getEntities().end(); itr++)
		{
			lua_pushnumber(state, index++);
			LuaEnvironment::pushObjectToLua<Entity>(state, itr->second, "jl.Entity");
			lua_settable(state, tableIndex);
		}

		return 1;
	}
	int LuaEnv_Scene::GetName(lua_State *state)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		lua_pushstring(state, scene->getName().c_str());

		return 1;
	}
	int LuaEnv_Scene::Detach(lua_State *state)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		// Second arg is the entity to detach
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 2, "jl.Entity");

		scene->detach(*entity);

		return 0;
	}
	int LuaEnv_Scene::Attach(lua_State *state)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		// Second arg is the entity to attach
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 2, "jl.Entity");

		scene->attach(*entity);

		return 0;
	}
	int LuaEnv_Scene::Contains(lua_State *state)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		// Second arg is the entity to query
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 2, "jl.Entity");

		lua_pushboolean(state, scene->contains(*entity));

		return 1;
	}
	void LuaEnv_Scene::exposeToLua(Game *game)
	{
		const luaL_Reg sceneFuncs[] = 
		{
			{ "GetEntities", GetEntities },
			{ "GetName", GetName },
			{ "Detach", Detach },
			{ "Attach", Attach },
			{ "Contains", Contains },
			{ NULL, NULL }
		};
		game->getLua().registerClassToLua("jl.Scene", sceneFuncs);
	}
};