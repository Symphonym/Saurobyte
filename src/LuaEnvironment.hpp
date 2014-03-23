#ifndef JL_LUAENVIRONMENT_HPP
#define JL_LUAENVIRONMENT_HPP

#include <Lua/lua.hpp>
#include <string>

namespace jl
{
	class LuaEnvironment
	{

	private:

		// Stop condition for arg pusher
		int pushArgsToLua()
		{
			return 0;
		};

		lua_State *m_luaContext;

	public:

		LuaEnvironment();
		~LuaEnvironment();

		// Utility function, simply converts userdata
		template<typename TType> static TType* convertUserdata(lua_State *state, int index, const std::string &metatableName)
		{
			TType **data = static_cast<TType**>(
				luaL_checkudata (state, index, metatableName.c_str()));

			return *data;
		};


		// Used when pushing multiple args to avoid ambiguity
		template<typename TType> struct LuaArg
		{
			const TType &data;
		};

		// Pushes a variable amount of values onto the Lua stack
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

		// Runs a given script and returns false if errors occurred
		bool runScript(const std::string &filePath);

		std::size_t getMemoryUsage() const;
		lua_State* getRaw();

	};
};

#endif