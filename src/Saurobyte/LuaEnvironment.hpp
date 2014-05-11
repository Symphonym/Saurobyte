#ifndef JL_LUAENVIRONMENT_HPP
#define JL_LUAENVIRONMENT_HPP

#include <Lua/lua.hpp>
#include <string>
#include <type_traits>
#include <memory>
#include <functional>
#include <vector>

namespace Saurobyte
{

	namespace internal
	{
		class LuaImpl;
	}

	class LuaEnvironment
	{

	public:

		typedef std::pair<std::string, std::function<int(LuaEnvironment&)> > LuaFunction; 

		LuaEnvironment();
		~LuaEnvironment();

		// Utility function, simply converts userdata
		template<typename TType> static TType* convertUserdata(lua_State *state, int index, const std::string &metatableName)
		{
			TType **data = static_cast<TType**>(
				luaL_checkudata (state, index, metatableName.c_str()));

			return *data;
		};

		template<typename ...TArgs> int pushArgs(TArgs... args)
		{
			return 1 + pushArg(args...);
		};

		// Pushes an object onto the Lua stack with the metatable specfied by 'className'
		template<typename TType> static void pushObject(lua_State *state, TType *newData, const std::string &className)
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


		void pushGlobalEnv();

		// Registers a number of functions in a metatable with the name specified by 'className'
		void registerClassToLua(const std::string &className, const luaL_Reg *funcs);
		void registerFunctions(const std::vector<LuaFunction> &funcs);

		// Runs a given script and returns false if errors occurred
		bool runScript(const std::string &filePath);
		void reportError();

		std::size_t getMemoryUsage() const;
		lua_State* getRaw();

	private:

		std::unique_ptr<internal::LuaImpl> m_lua; 
		//
		lua_State *m_luaContext;

						// Stop condition for arg pusher
		int pushArg()
		{
			return 0;
		};
		// Pushes a variable amount of values onto the Lua stack
		template<typename TType, typename ...TArgs> 
			typename std::enable_if<std::is_same<bool, TType>::value, int>::type pushArg(TType arg, TArgs... args)
		{
			pushBool(arg);
			return 1 + pushArg(args...);
		};
		template<typename TType, typename ...TArgs> 
			typename std::enable_if<std::is_same<std::string, TType>::value, int>::type pushArg(TType arg, TArgs... args)
		{
			pushString(arg);
			return 1 + pushArg(args...);
		};
		template<typename TType, typename ...TArgs> 
			typename std::enable_if<std::is_arithmetic<TType>::value && !std::is_same<bool, TType>::value, int>::type pushArg(TType arg, TArgs... args)
		{
			pushNumber(arg);
			return 1 + pushArg(args...);
		};

		void pushBool(bool value);
		void pushNumber(double value);
		void pushString(const std::string &value);

	};
};

#endif