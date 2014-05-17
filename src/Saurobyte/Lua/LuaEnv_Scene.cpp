
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
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Game.hpp>

namespace Saurobyte
{
	int LuaEnv_Scene::GetEntities(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = env.toObject<Scene*>("Saurobyte_Scene");

		//lua_createtable(state, scene->getEntities().size(), 0);
		env.pushTable();
		//int tableIndex = lua_gettop(state);

		int index = 1;
		for(auto itr = scene->getEntities().begin(); itr != scene->getEntities().end(); itr++)
		{
			//lua_pushnumber(state, index++);
			env.pushObject<Entity*>(itr->second, "Saurobyte_Entity");
			env.tableWrite(index);
			//lua_settable(state, tableIndex);
		}

		return 1;
	}
	int LuaEnv_Scene::GetName(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = env.toObject<Scene*>("Saurobyte_Scene");

		env.pushArgs(scene->getName());

		return 1;
	}
	int LuaEnv_Scene::Detach(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = env.toObject<Scene*>("Saurobyte_Scene");

		// Second arg is the entity to detach
		Entity *entity = env.toObject<Entity*>("Saurobyte_Entity");

		scene->detach(*entity);

		return 0;
	}
	int LuaEnv_Scene::Attach(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = env.toObject<Scene*>("Saurobyte_Scene");

		// Second arg is the entity to attach
		Entity *entity = env.toObject<Entity*>("Saurobyte_Entity");

		scene->attach(*entity);

		return 0;
	}
	int LuaEnv_Scene::Contains(LuaEnvironment &env)
	{
		// First arg is self
		Scene* scene = env.toObject<Scene*>("Saurobyte_Scene");

		// Second arg is the entity to query
		Entity *entity = env.toObject<Entity*>("Saurobyte_Entity");

		env.pushArgs(scene->contains(*entity));

		return 1;
	}
	void LuaEnv_Scene::exposeToLua(Game *game)
	{
		/*const luaL_Reg sceneFuncs[] = 
		{
			{ "GetEntities", GetEntities },
			{ "GetName", GetName },
			{ "Detach", Detach },
			{ "Attach", Attach },
			{ "Contains", Contains },
			{ NULL, NULL }
		};*/

		LuaEnvironment &env = game->getLua();
		env.createClass("Saurobyte_Scene",
		{
			{ "GetEntities", GetEntities },
			{ "GetName", GetName },
			{ "Detach", Detach },
			{ "Attach", Attach },
			{ "Contains", Contains }
		});
		//game->getLua().createClass("Saurobyte_Scene", sceneFuncs);
	}
};