
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


#include <Saurobyte/Lua/LuaEnv_Component.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Game.hpp>

namespace Saurobyte
{		


	// Getter for component values
	int LuaEnv_Component::GetComponentValue(LuaEnvironment &env)
	{
		// First argument is self
		BaseComponent *comp = env.toObject<BaseComponent*>("Saurobyte_Component");

		// Second argument is value identifier
		std::string valueName = env.toString();

		return comp->onLuaGet(valueName, env);
	}

	// Setter for component values
	int LuaEnv_Component::SetComponentValue(LuaEnvironment &env)
	{
		// First argument is self
		BaseComponent *comp = env.toObject<BaseComponent*>("Saurobyte_Component");

		// Second argument is value identifier
		std::string valueName = env.toString();

		// Third values and so forth are optional arguments handled in
		// the component. But we will remove first and second args
		// to make it easier on the users end.
		//lua_remove(state, 1); // Pop two front elements
		//lua_remove(state, 1);

		comp->onLuaSet(valueName, env);
		return 0;
	}

	// Get component name
	int LuaEnv_Component::GetComponentName(LuaEnvironment &env)
	{
		// First argument is self
		BaseComponent *comp = env.toObject<BaseComponent*>("Saurobyte_Component");

		env.pushArgs(comp->getName());
		return 1;
	}

	 void LuaEnv_Component::exposeToLua(Game *game)
	 {
	 	LuaEnvironment &env = game->getLua();
	 	env.createClass("Saurobyte_Component",
	 	{
			{ "GetValue", GetComponentValue },
			{ "SetValue", SetComponentValue },
			{ "GetName", GetComponentName }
	 	});
	 	/*const luaL_Reg componentFuncs[] = 
		{
			{ "GetValue", GetComponentValue },
			{ "SetValue", SetComponentValue },
			{ "GetName", GetComponentName },
			{ NULL, NULL }
		};*/
		//game->getLua().createClass("Saurobyte_Component", componentFuncs);
	}
};