
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

#ifndef SAUROBYTE_LUAENV_SCENE_HPP
#define SAUROBYTE_LUAENV_SCENE_HPP

namespace Saurobyte
{
	class Game;
	class LuaEnvironment;
	class LuaEnv_Scene
	{

	private:

		// Get table of entities in the scene
		static int GetEntities(LuaEnvironment &env);

		// Get name of the scene
		static int GetName(LuaEnvironment &env);

		// Detach an entity from the scene
		static int Detach(LuaEnvironment &env);

		// Attach an entity to the scene
		static int Attach(LuaEnvironment &env);

		// Check if an entity is within the scene
		static int Contains(LuaEnvironment &env);


	public:
		
		static void exposeToLua(Game *game);
	};
};

#endif