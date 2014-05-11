#ifndef JL_LUAENV_COMPONENT_HPP
#define JL_LUAENV_COMPONENT_HPP


class lua_State;
namespace Saurobyte
{
	class Game;
	class LuaEnv_Component
	{

	private:

		// Getter for component values
		static int GetComponentValue(lua_State *state);

		// Setter for component values
		static int SetComponentValue(lua_State *state);

		// Get component name
		static int GetComponentName(lua_State *state);
		

	public:

		static void exposeToLua(Game *game);
	};
};

#endif