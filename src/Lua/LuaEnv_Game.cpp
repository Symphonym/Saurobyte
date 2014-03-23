#include "LuaEnv_Game.hpp"
#include "Game.hpp"

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


	 void LuaEnv_Game::exposeToLua(Game *game)
	 {
	 	const luaL_Reg gameFuncs[] = 
		{
			{ "ChangeScene", ChangeScene },
			{ "DeleteScene", DeleteScene },
			{ "GetTotalEntityCount", GetTotalEntityCount },
			{ "MoveCamera", MoveCamera },
			{ NULL, NULL }
		};
		game->getLua().registerClassToLua("jl.Game", gameFuncs);
	 }
};