#ifndef JL_LUAENV_INPUT_HPP
#define JL_LUAENV_INPUT_HPP


class lua_State;
namespace jl
{
	class Game;
	class LuaEnv_Input
	{

	private:

		// Get mouse position
		static int GetMousePos(lua_State *state);

		// Mouse pressed
		static int IsMousePressed(lua_State *state);
		
		// Keyboard key pressed
		static int IsKeyPressed(lua_State *state);
		

	public:

		static void exposeToLua(Game *game);
	};
};

#endif