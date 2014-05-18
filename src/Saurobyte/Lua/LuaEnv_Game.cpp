
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

#include <Saurobyte/Lua/LuaEnv_Game.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Game.hpp>

namespace Saurobyte
{
	int LuaEnv_Game::ChangeScene(LuaEnvironment &env)
	{

		// First arg is self
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();
			std::string sceneName = env.readArg<std::string>();
			game->changeScene(sceneName.c_str());

		}
		//Game* game = env.toObject<Game*>(state, 1, "jl.Game");

		// Second argument is scene name
		//std::string sceneName = luaL_checkstring(state, 2);

		return 0;
	}
	int LuaEnv_Game::DeleteScene(LuaEnvironment &env)
	{

 		// First arg is self
 		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();
			std::string sceneName = env.readArg<std::string>();
			game->getScenePool().deleteScene(sceneName.c_str());
		}
		//Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		/// Second argument is scene name
		//std::string sceneName = luaL_checkstring(state, 2);

		//lua_settop(state, 0);


		return 0;
	}
	int LuaEnv_Game::GetActiveScene(LuaEnvironment &env)
	{
		// First arg is self
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();

			Scene *activeScene = game->getScenePool().getActiveScene();
			
			if(activeScene == nullptr)
				env.pushNil();
			else
				env.pushObject<Scene*>(activeScene, "Saurobyte_Scene");
		}

		//Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		//Scene *activeScene = game->getScenePool().getActiveScene();



		return 1;
	}
	int LuaEnv_Game::GetTotalEntityCount(LuaEnvironment &env)
	{
		// First arg is self
		//Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();
			env.pushArgs(game->getEntityPool().getEntityCount());
		}

		//lua_pushnumber(state, game->getEntityPool().getEntityCount());

		return 1;
	}
	int LuaEnv_Game::MoveCamera(LuaEnvironment &env)
	{
		// First arg is self
		//Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();

			// 2nd arg is X offset, 3rd arg is Y offset, 4th arg is Z offset
			float xOff = env.readArg<float>();
			float yOff = env.readArg<float>();
			float zOff = env.readArg<float>();
		}

		

		//game->getScenePool().getActiveScene()->getCamera().move(glm::vec3(xOff, yOff, zOff));

		return 0;
	}
 	int LuaEnv_Game::GetWindowWidth(LuaEnvironment &env)
 	{
 		// First arg is self
		//Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();
			env.pushArgs(game->getWindow().getSize().x);
		}


		//lua_pushnumber(state, game->getWindow().getSize().x);

		return 1;
 	}
	int LuaEnv_Game::GetWindowHeight(LuaEnvironment &env)
	{
		// First arg is self
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Game *game = env.readStack<Game*>();
			env.pushArgs(game->getWindow().getSize().y);
		}
		//Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		//lua_settop(state, 0);

		//lua_pushnumber(state, game->getWindow().getSize().y);

		return 1;
	}


	void LuaEnv_Game::exposeToLua(Game *game)
	{
		/*const luaL_Reg gameFuncs[] = 
		{
			{ "ChangeScene", ChangeScene },
			{ "DeleteScene", DeleteScene },
			{ "GetActiveScene", GetActiveScene },
			{ "GetTotalEntityCount", GetTotalEntityCount },
			{ "MoveCamera", MoveCamera },
			{ "GetWindowWidth", GetWindowWidth },
			{ "GetWindowHeight", GetWindowHeight },
			{ NULL, NULL }
		};*/
		//game->getLua().createClass("jl.Game", gameFuncs);
		//
		LuaEnvironment &env = game->getLua();
		env.registerFunction({ "ChangeScene", ChangeScene });
		env.registerFunction({ "DeleteScene", DeleteScene });
		env.registerFunction({ "GetActiveScene", GetActiveScene });
		env.registerFunction({ "GetTotalEntityCount", GetTotalEntityCount });
		env.registerFunction({ "MoveCamera", MoveCamera });
		env.registerFunction({ "GetWindowWidth", GetWindowWidth });
		env.registerFunction({ "GetWindowHeight", GetWindowHeight });


		// Create global game object in Lua environment
		//LuaEnvironment &env = game->getLua().getRaw();
		//LuaEnvironment::pushObject<Game>(state, game, "jl.Game");
		//lua_setglobal(state, "game");
	}
};