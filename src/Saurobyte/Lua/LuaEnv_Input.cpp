#include <Saurobyte/Lua/LuaEnv_Input.hpp>
#include <Saurobyte/Game.hpp>
#include <SDL2/SDL.h>

namespace Saurobyte
{		

	int LuaEnv_Input::GetMousePos(lua_State *state)
	{
		int mouseX = -1, mouseY = -1;
		SDL_GetMouseState(&mouseX, &mouseY);

		lua_settop(state, 0);

		lua_pushnumber(state, mouseX);
		lua_pushnumber(state, mouseY);
		return 2;
	}
	int LuaEnv_Input::IsMousePressed(lua_State *state)
	{
		// First argument is mouse key
		std::string mouseKeyName = luaL_checkstring(state, 1);

		lua_settop(state, 0);

		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
		if(mouseKeyName == "Left")
			lua_pushboolean(state, mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
		else if(mouseKeyName == "Right")
			lua_pushboolean(state, mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));
		else if(mouseKeyName == "Middle")
			lua_pushboolean(state, mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE));


		return 1;
	}
	int LuaEnv_Input::IsKeyPressed(lua_State *state)
	{
		// First argument is keyboard key
		std::string keyboardKeyName = luaL_checkstring(state, 1);

		lua_settop(state, 0);

		SDL_Keycode keyCode = SDL_GetKeyFromName(keyboardKeyName.c_str());

		if(keyCode == SDLK_UNKNOWN)
			lua_pushboolean(state, 0);
		else
		{
			const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
			lua_pushboolean(state, keyboardState[SDL_GetScancodeFromKey(keyCode)]);
		}

		return 1;
	}


	 void LuaEnv_Input::exposeToLua(Game *game)
	 {
	 	const luaL_Reg inputFuncs[] = 
		{
			{ "GetMousePos", GetMousePos },
			{ "IsMousePressed", IsMousePressed },
			{ "IsKeyPressed", IsKeyPressed },
			{ NULL, NULL }
		};

		lua_State *state = game->getLua().getRaw();

		// Register input functions to global environment
		lua_pushglobaltable(state);
		luaL_setfuncs(state, inputFuncs, 0);	 }
};