#ifndef JL_LUASYSTEM_HPP
#define JL_LUASYSTEM_HPP

#include <string>
#include <Lua/lua.hpp>
#include "System.hpp"
#include "IdentifierTypes.hpp"
#include <SDL2/SDL.h>
namespace jl
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


		lua_State *m_luaContext;
		LuaEnvironment &m_luaEnv;

		// Scripts subscribed to events
		std::unordered_map<std::string, std::vector<Entity*> > m_subscribedScripts;

		void runScript(Entity &entity);

	public:

		LuaSystem(Game *game);
		~LuaSystem();

		// Subscribes the entity to the specified event, so its scripts receives them
		void subscribeEntity(Entity &entity, const std::string &eventName);
		void unsubscribeEntity(Entity &entity, const std::string &eventName);

		virtual void onMessage(jl::Message *message);

		virtual void processEntity(Entity &entity);
		virtual void postProcess();

		virtual void onEntityAdded(Entity &entity);
		virtual void onEntityRemoved(Entity &entity);
		virtual void onSystemCleared();
	};
};

#endif