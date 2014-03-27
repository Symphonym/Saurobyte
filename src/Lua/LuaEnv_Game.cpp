#include "LuaEnv_Game.hpp"
#include "Game.hpp"
#include "Logger.hpp"

namespace jl
{
	 int LuaEnv_Game::ChangeScene(lua_State *state)
	 {
	 	// First arg is self
		Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		// Second argument is scene name
		std::string sceneName = luaL_checkstring(state, 2);

		lua_settop(state, 0);

		game->changeScene(sceneName.c_str());

		return 0;
	 }
	 int LuaEnv_Game::DeleteScene(lua_State *state)
	 {
 		// First arg is self
		Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		// Second argument is scene name
		std::string sceneName = luaL_checkstring(state, 2);

		lua_settop(state, 0);

		game->getScenePool().deleteScene(sceneName.c_str());

		return 0;
	 }
	 int LuaEnv_Game::GetTotalEntityCount(lua_State *state)
	 {
	 	// First arg is self
		Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		lua_settop(state, 0);

		lua_pushnumber(state, game->getEntityPool().getEntityCount());

		return 1;
	 }
	 int LuaEnv_Game::MoveCamera(lua_State *state)
	 {
		// First arg is self
		Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		// 2nd arg is X offset, 3rd arg is Y offset, 4th arg is Z offset
		float xOff = luaL_checknumber(state, 2);
		float yOff = luaL_checknumber(state, 3);
		float zOff = luaL_checknumber(state, 4);

		lua_settop(state, 0);

		game->getScenePool().getActiveScene()->getCamera().move(glm::vec3(xOff, yOff, zOff));

		return 0;
	 }
 	int LuaEnv_Game::GetWindowWidth(lua_State *state)
 	{
 		// First arg is self
		Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		lua_settop(state, 0);

		lua_pushnumber(state, game->getWindow().getWidth());

		return 1;
 	}
	int LuaEnv_Game::GetWindowHeight(lua_State *state)
	{
		// First arg is self
		Game* game = LuaEnvironment::convertUserdata<Game>(state, 1, "jl.Game");

		lua_settop(state, 0);

		lua_pushnumber(state, game->getWindow().getHeight());

		return 1;
	}


	 void LuaEnv_Game::exposeToLua(Game *game)
	 {
	 	const luaL_Reg gameFuncs[] = 
		{
			{ "ChangeScene", ChangeScene },
			{ "DeleteScene", DeleteScene },
			{ "GetTotalEntityCount", GetTotalEntityCount },
			{ "MoveCamera", MoveCamera },
			{ "GetWindowWidth", GetWindowWidth },
			{ "GetWindowHeight", GetWindowHeight },
			{ NULL, NULL }
		};
		game->getLua().registerClassToLua("jl.Game", gameFuncs);

		// Create global game object in Lua environment
		lua_State *state = game->getLua().getRaw();
		LuaEnvironment::pushObjectToLua<Game>(state, game, "jl.Game");
		lua_setglobal(state, "game");
	 }
};