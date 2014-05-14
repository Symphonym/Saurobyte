
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

#include <Saurobyte/Lua/LuaEnv_Scene.hpp>
#include <Saurobyte/Game.hpp>

namespace Saurobyte
{
	int LuaEnv_Scene::GetEntities(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		lua_createtable(state, scene->getEntities().size(), 0);
		int tableIndex = lua_gettop(state);

		int index = 1;
		for(auto itr = scene->getEntities().begin(); itr != scene->getEntities().end(); itr++)
		{
			lua_pushnumber(state, index++);
			LuaEnvironment::pushObject<Entity>(state, itr->second, "jl.Entity");
			lua_settable(state, tableIndex);
		}

		return 1;
	}
	int LuaEnv_Scene::GetName(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		lua_pushstring(state, scene->getName().c_str());

		return 1;
	}
	int LuaEnv_Scene::Detach(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		// Second arg is the entity to detach
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 2, "jl.Entity");

		scene->detach(*entity);

		return 0;
	}
	int LuaEnv_Scene::Attach(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = LuaEnvironment::convertUserdata<Scene>(state, 1, "jl.Scene");

		// Second arg is the entity to attach
		Entity* entity = LuaEnvironment::convertUserdata<Entity>(state, 2, "jl.Entity");

		scene->attach(*entity);

		return 0;
	}
	int LuaEnv_Scene::Contains(LuaEnvironment &env)
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
		//game->getLua().createClass("jl.Scene", sceneFuncs);
	}
};