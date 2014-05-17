
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


#include <Saurobyte/Lua/LuaEnv_Input.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Game.hpp>
#include <SDL2/SDL.h>

namespace Saurobyte
{		

	int LuaEnv_Input::GetMousePos(LuaEnvironment &env)
	{
		int mouseX = -1, mouseY = -1;
		SDL_GetMouseState(&mouseX, &mouseY);

		env.pushArgs(mouseX, mouseY);

		//lua_settop(state, 0);

		//lua_pushnumber(state, mouseX);
		//lua_pushnumber(state, mouseY);
		return 2;
	}
	int LuaEnv_Input::IsMousePressed(LuaEnvironment &env)
	{
		// First argument is mouse key
		std::string mouseKeyName = env.toString();

		//lua_settop(state, 0);

		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
		if(mouseKeyName == "Left")
			env.pushArgs(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
		else if(mouseKeyName == "Right")
			env.pushArgs(mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));
		else if(mouseKeyName == "Middle")
			env.pushArgs(mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE));


		return 1;
	}
	int LuaEnv_Input::IsKeyPressed(LuaEnvironment &env)
	{
		// First argument is keyboard key
		std::string keyboardKeyName = env.toString();

		//lua_settop(state, 0);

		SDL_Keycode keyCode = SDL_GetKeyFromName(keyboardKeyName.c_str());

		if(keyCode == SDLK_UNKNOWN)
			env.pushArgs(false);
		else
		{
			const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
			env.pushArgs(static_cast<bool>(keyboardState[SDL_GetScancodeFromKey(keyCode)]));
		}

		return 1;
	}


	 void LuaEnv_Input::exposeToLua(Game *game)
	 {
	 	LuaEnvironment &env = game->getLua();
	 	env.registerFunction({"GetMousePos", GetMousePos });
		env.registerFunction({"IsMousePressed", IsMousePressed });
		env.registerFunction({"IsKeyPressed", IsKeyPressed });
	 	/*const luaL_Reg inputFuncs[] = 
		{
			{ "GetMousePos", GetMousePos },
			{ "IsMousePressed", IsMousePressed },
			{ "IsKeyPressed", IsKeyPressed },
			{ NULL, NULL }
		};

		LuaEnvironment &env = game->getLua().getRaw();

		// Register input functions to global environment
		lua_pushglobaltable(state);
		luaL_setfuncs(state, inputFuncs, 0);*/
	}
};