#ifndef JL_LUASYSTEM_HPP
#define JL_LUASYSTEM_HPP

#include <string>
#include <Lua/lua.hpp>
#include "System.hpp"
#include "IdentifierTypes.hpp"
#include <SDL2/SDL.h>
namespace jl
{
	class LuaSystem : public System<LuaSystem>
	{
	private:

		// Lua utility functions
		template<typename TType> static TType* convertUserdata(lua_State *state, int index, const std::string &metatableName)
		{
			TType **data = static_cast<TType**>(
				luaL_checkudata (state, index, metatableName.c_str()));

			return *data;
		};
		template<typename TType> static void pushUserdataPointer(lua_State *state, TType *newData, const std::string &metatableName)
		{
			// Associate userdata with empty metatable, this is done to more easily
			// accomplish type safety
			void *data = lua_newuserdata(state, sizeof(TType*));
			TType **dataPtr = static_cast<TType**>(data);
			*dataPtr = newData;
			luaL_newmetatable(state, metatableName.c_str());
			lua_setmetatable(state, -2);
		};

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

		template<typename TType> struct LuaArg
		{
			const TType &data;
		};
		
		int pushArgsToLua()
		{
			return 0;
		}
		template<typename ...TArgs> int pushArgsToLua(const LuaArg<bool> &arg, TArgs... args)
		{
			lua_pushboolean(m_luaContext, arg.data);
			return 1 + pushArgsToLua(args...);
		};
		template<typename ...TArgs> int pushArgsToLua(const LuaArg<std::string> &arg, TArgs... args)
		{
			lua_pushstring(m_luaContext, arg.data.c_str());
			return 1 + pushArgsToLua(args...);
		};
		template<typename ...TArgs> int pushArgsToLua(const LuaArg<float> &arg, TArgs... args)
		{
			lua_pushnumber(m_luaContext, arg.data);
			return 1 + pushArgsToLua(args...);
		};

		void reportLuaError(Entity &entity, int errorIndex);
		void createLuaEnvironment();
		void runLuaFile(Entity &entity);

		// Pushes an object onto the Lua stack with the metatable specfied by 'className'
		template<typename TType> static void pushObjectToLua(lua_State *state, TType *newData, const std::string &className)
		{
			luaL_getmetatable(state, className.c_str());
			int metaTable = lua_gettop(state);

			// Create userdata
			void *data = lua_newuserdata(state, sizeof(TType*));
			TType **dataPtr = static_cast<TType**>(data);
			*dataPtr = newData;
			lua_pushvalue(state, -1);

			// Associate self with C++ object
			lua_setfield(state, metaTable, "__self");
			lua_remove(state, metaTable);

			luaL_setmetatable(state, className.c_str());
		}
		// Registers a number of functions in a metatable with the name specified by 'className'
		void registerClassToLua(const std::string &className, const luaL_Reg *funcs);

		lua_State *m_luaContext;

		// Scripts subscribed to events
		std::unordered_map<std::string, std::vector<Entity*> > m_subscribedScripts;


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

		std::size_t getMemoryUsage() const;
	};
};

#endif