
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


#ifndef SAUROBYTE_LUASYSTEM_HPP
#define SAUROBYTE_LUASYSTEM_HPP

#include <string>
#include <Lua/lua.hpp>
#include <SDL2/SDL.h>
#include <Saurobyte/System.hpp>
#include <Saurobyte/IdentifierTypes.hpp>

namespace Saurobyte
{
	class LuaEnvironment;
	class LuaSystem : public System<LuaSystem>
	{
	private:

	/*		struct Tester
			{
				int numA, numB, numC;
				const char *strA;

				Tester(int a, int b, int c, const char *d)
					:
					numA(a),
					numB(b),
					numC(c),
					strA(d)
				{

				}
			};

		template<int Count, typename Arg0, typename... ArgN> class reverse_variadic
		{
		     
		public:
			std::tuple<ArgN...> create(lua_State *state, ArgN... args)
			{
				//if(lua_gettop(state) >= 1)
				//{
					if(lua_isnumber(state, -1))
					{
						int numb = lua_tonumber(state, -1);
						lua_pop(state, 1);
						
						reverse_variadic<Count-1, Arg0, ArgN..., int> inner;
						return inner.create(state, args..., numb);
					}
					else
					{
						const char *str = lua_tostring(state, -1);
						lua_pop(state, 1);
						
						reverse_variadic<Count-1, Arg0, ArgN..., const char*> inner;
						return inner.create(state, args..., str);
					}
				//}
				//else
				//{
				//	reverse_variadic<0, Arg0, ArgN...> inner;
				//	return inner.create(state, args...);
				//} 


			}
		};

		template<typename Arg0, typename... ArgN> class reverse_variadic<0, Arg0, ArgN...>
		{
		public:
			std::tuple<ArgN...> create(lua_State *state, ArgN... args)
			{
				return std::make_tuple(args...);
			}
		};*/


		// Scripts subscribed to events
		std::unordered_map<std::string, std::vector<Entity*> > m_subscribedScripts;

		void runScript(Entity &entity);

	public:

		LuaSystem(Game *game);
		~LuaSystem();

		// Subscribes the entity to the specified event, so its scripts receives them
		void subscribeEntity(Entity &entity, const std::string &eventName);
		void unsubscribeEntity(Entity &entity, const std::string &eventName);

		virtual void onMessage(Message *message);

		virtual void processEntity(Entity &entity);
		virtual void postProcess();

		virtual void onAttach(Entity &entity);
		virtual void onDetach(Entity &entity);
		virtual void onKill(Entity &entity);
		virtual void onClear();
	};
};

#endif