
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

#include <Saurobyte/Lua/LuaEnv_Engine.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Engine.hpp>

namespace Saurobyte
{
	int LuaEnv_Engine::ChangeScene(LuaEnvironment &env)
	{

		// First arg is self
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");
			std::string sceneName = env.readArg<std::string>();
			engine->changeScene(sceneName.c_str());

		}
		//Engine* engine = env.toObject<Engine*>(state, 1, "jl.Engine");

		// Second argument is scene name
		//std::string sceneName = luaL_checkstring(state, 2);

		return 0;
	}
	int LuaEnv_Engine::DeleteScene(LuaEnvironment &env)
	{

 		// First arg is self
 		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");
			std::string sceneName = env.readArg<std::string>();
			engine->getScenePool().deleteScene(sceneName.c_str());
		}
		//Engine* engine = LuaEnvironment::convertUserdata<Engine>(state, 1, "jl.Engine");

		/// Second argument is scene name
		//std::string sceneName = luaL_checkstring(state, 2);

		//lua_settop(state, 0);


		return 0;
	}
	int LuaEnv_Engine::GetActiveScene(LuaEnvironment &env)
	{
		// First arg is self
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");

			Scene *activeScene = engine->getScenePool().getActiveScene();
			
			if(activeScene == nullptr)
				env.pushNil();
			else
				env.pushObject<Scene*>(activeScene, "Saurobyte_Scene");
		}

		//Engine* engine = LuaEnvironment::convertUserdata<Engine>(state, 1, "jl.Engine");

		//Scene *activeScene = engine->getScenePool().getActiveScene();



		return 1;
	}
	int LuaEnv_Engine::GetTotalEntityCount(LuaEnvironment &env)
	{
		// First arg is self
		//Engine* engine = LuaEnvironment::convertUserdata<Engine>(state, 1, "jl.Engine");
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");
			env.pushArgs(engine->getEntityPool().getEntityCount());
		}

		//lua_pushnumber(state, engine->getEntityPool().getEntityCount());

		return 1;
	}
	int LuaEnv_Engine::MoveCamera(LuaEnvironment &env)
	{
		// First arg is self
		//Engine* engine = LuaEnvironment::convertUserdata<Engine>(state, 1, "jl.Engine");
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");

			// 2nd arg is X offset, 3rd arg is Y offset, 4th arg is Z offset
			float xOff = env.readArg<float>();
			float yOff = env.readArg<float>();
			float zOff = env.readArg<float>();
		}

		

		//engine->getScenePool().getActiveScene()->getCamera().move(glm::vec3(xOff, yOff, zOff));

		return 0;
	}
 	int LuaEnv_Engine::GetWindowWidth(LuaEnvironment &env)
 	{
 		// First arg is self
		//Engine* engine = LuaEnvironment::convertUserdata<Engine>(state, 1, "jl.Engine");
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");
			env.pushArgs(engine->getWindow().getSize().x);
		}


		//lua_pushnumber(state, engine->getWindow().getSize().x);

		return 1;
 	}
	int LuaEnv_Engine::GetWindowHeight(LuaEnvironment &env)
	{
		// First arg is self
		if(env.readGlobal("SAUROBYTE_GAME"))
		{
			Engine *engine = env.readStack<Engine*>("Saurobyte_Engine");
			env.pushArgs(engine->getWindow().getSize().y);
		}
		//Engine* engine = LuaEnvironment::convertUserdata<Engine>(state, 1, "jl.Engine");

		//lua_settop(state, 0);

		//lua_pushnumber(state, engine->getWindow().getSize().y);

		return 1;
	}


	void LuaEnv_Engine::exposeToLua(Engine *engine)
	{
		/*const luaL_Reg engineFuncs[] = 
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
		//engine->getLua().createClass("jl.Engine", engineFuncs);
		//
		LuaEnvironment &env = engine->getLua();
		env.registerFunction({ "ChangeScene", ChangeScene });
		env.registerFunction({ "DeleteScene", DeleteScene });
		env.registerFunction({ "GetActiveScene", GetActiveScene });
		env.registerFunction({ "GetTotalEntityCount", GetTotalEntityCount });
		env.registerFunction({ "MoveCamera", MoveCamera });
		env.registerFunction({ "GetWindowWidth", GetWindowWidth });
		env.registerFunction({ "GetWindowHeight", GetWindowHeight });

		env.pushObject<Engine*>(engine, "Saurobyte_Engine");
		env.writeGlobal("SAUROBYTE_GAME");
		// Create global engine object in Lua environment
		//LuaEnvironment &env = engine->getLua().getRaw();
		//LuaEnvironment::pushObject<Engine>(state, engine, "jl.Engine");
		//lua_setglobal(state, "engine");
	}
};