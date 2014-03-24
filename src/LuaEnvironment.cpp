#include "LuaEnvironment.hpp"
#include "Logger.hpp"

namespace jl
{


	LuaEnvironment::LuaEnvironment()
	{

		// Create lua state and environment
		m_luaContext = luaL_newstate();
		luaL_openlibs(m_luaContext);
	}
	LuaEnvironment::~LuaEnvironment()
	{
		lua_close(m_luaContext);
	}

	void LuaEnvironment::registerClassToLua(const std::string &className, const luaL_Reg *funcs)
	{
		luaL_newmetatable(m_luaContext, className.c_str());
		int metaTable = lua_gettop(m_luaContext);

		luaL_setfuncs(m_luaContext, funcs, 0);

		lua_pushvalue(m_luaContext, -1);
		lua_setfield(m_luaContext, metaTable, "__index");
	}


	bool LuaEnvironment::runScript(const std::string &filePath)
	{
		if(luaL_dofile(m_luaContext, filePath.c_str()))
		{
			reportError();
			return false;
		}
		else
			return true;
	}
	void LuaEnvironment::reportError()
	{
		JL_ERROR_LOG("LUA ERROR: %s", lua_tostring(m_luaContext, -1));
	}

	std::size_t LuaEnvironment::getMemoryUsage() const
	{
		return lua_gc(m_luaContext, LUA_GCCOUNT, 0);
	}
	lua_State* LuaEnvironment::getRaw()
	{
		return m_luaContext;
	}


};