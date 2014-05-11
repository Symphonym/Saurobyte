#ifndef JL_LUAENV_SCENE_HPP
#define JL_LUAENV_SCENE_HPP

class lua_State;
namespace Saurobyte
{
	class Game;
	class LuaEnv_Scene
	{

	private:

		// Get table of entities in the scene
		static int GetEntities(lua_State *state);

		// Get name of the scene
		static int GetName(lua_State *state);

		// Detach an entity from the scene
		static int Detach(lua_State *state);

		// Attach an entity to the scene
		static int Attach(lua_State *state);

		// Check if an entity is within the scene
		static int Contains(lua_State *state);


	public:
		
		static void exposeToLua(Game *game);
	};
};

#endif