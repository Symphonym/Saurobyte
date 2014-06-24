
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


#ifndef SAUROBYTE_LUAENV_ENTITY_HPP
#define SAUROBYTE_LUAENV_ENTITY_HPP


namespace Saurobyte
{
	class Engine;
	class LuaEnvironment;
	class LuaEnv_Entity
	{

	private:

		// Add component to entity
		//static int AddComponent = [] (LuaEnvironment &env);

		// Create a component TODO with creatinf and add n shit
		//static int CreateComponent = [] (LuaEnvironment &env);

		// Getter for components themselves
		static int GetComponent(LuaEnvironment &env);

		// Get component count of entity
		static int GetComponentCount(LuaEnvironment &env);

		// Remove components
		static int RemoveComponent(LuaEnvironment &env);

		// Has component
		static int HasComponent(LuaEnvironment &env);

		// Enable entity
		static int EnableEntity(LuaEnvironment &env);

		// Disable entity
		static int DisableEntity(LuaEnvironment &env);

		// Kill entity
		static int KillEntity(LuaEnvironment &env);

		// Get ID of entity
		static int GetID(LuaEnvironment &env);


		// Subscribe entity to an event
		static int SubscribeEvent(LuaEnvironment &env);

		// Unsubscribe entity to an event
		static int UnsubscribeEvent(LuaEnvironment &env);

	public:

		static void exposeToLua(Engine *engine);
	};
};

#endif