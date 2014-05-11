#include <Saurobyte/Lua/LuaEnv_Component.hpp>
#include <Saurobyte/Game.hpp>

namespace Saurobyte
{		


	// Getter for component values
	int LuaEnv_Component::GetComponentValue(lua_State *state)
	{
		// First argument is self
		BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, 1, "jl.Component");

		// Second argument is value identifier
		std::string valueName = luaL_checkstring(state, 2);

		return comp->onLuaGet(valueName, state);
	}

	// Setter for component values
	int LuaEnv_Component::SetComponentValue(lua_State *state)
	{
		// First argument is self
		BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, 1, "jl.Component");

		// Second argument is value identifier
		std::string valueName = luaL_checkstring(state, 2);

		// Third values and so forth are optional arguments handled in
		// the component. But we will remove first and second args
		// to make it easier on the users end.
		lua_remove(state, 1); // Pop two front elements
		lua_remove(state, 1);

		comp->onLuaSet(valueName, state);
		return 0;
	}

	// Get component name
	int LuaEnv_Component::GetComponentName(lua_State *state)
	{
		// First argument is self
		BaseComponent *comp = LuaEnvironment::convertUserdata<BaseComponent>(state, 1, "jl.Component");

		lua_pushstring(state, comp->getName().c_str());
		return 1;
	}

	 void LuaEnv_Component::exposeToLua(Game *game)
	 {
	 	const luaL_Reg componentFuncs[] = 
		{
			{ "GetValue", GetComponentValue },
			{ "SetValue", SetComponentValue },
			{ "GetName", GetComponentName },
			{ NULL, NULL }
		};
		game->getLua().createClass("jl.Component", componentFuncs);
	}
};