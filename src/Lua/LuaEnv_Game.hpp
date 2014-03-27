#ifndef JL_LUAENV_GAME_HPP
#define JL_LUAENV_GAME_HPP

class lua_State;
namespace jl
{
	class Game;
	class LuaEnv_Game
	{

	private:

		// Change scene
		static int ChangeScene(lua_State *state);

		// Delete scene
		static int DeleteScene(lua_State *state);

		// Get total entity count
		static int GetTotalEntityCount(lua_State *state);

		// Move scene camera
		static int MoveCamera(lua_State *state);

		// Get game window width
		static int GetWindowWidth(lua_State *state);

		// Get game window height
		static int GetWindowHeight(lua_State *state);

		// Set game window gamma
		static int SetWindowGamma(lua_State *state);


	public:
		
		static void exposeToLua(Game *game);
	};
};

#endif