#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/LuaImpl.hpp>
#include <Lua/lua.hpp>

namespace Saurobyte
{


	LuaEnvironment::LuaEnvironment()
	{

		m_lua = std::unique_ptr<internal::LuaImpl>(new internal::LuaImpl());

		// Create lua state and environment
		m_luaContext = luaL_newstate();
		luaL_openlibs(m_luaContext);
	}
	LuaEnvironment::~LuaEnvironment()
	{
		lua_close(m_luaContext);
	}

	void LuaEnvironment::pushBool(bool value)
	{
		lua_pushboolean(m_lua->state, value);
	}
	void LuaEnvironment::pushNumber(double value)
	{
		lua_pushnumber(m_lua->state, value);
	}
	void LuaEnvironment::pushString(const std::string &value)
	{
		lua_pushstring(m_lua->state, value.c_str());
	}
	void* LuaEnvironment::pushMemory(std::size_t sizeInBytes)
	{
		return lua_newuserdata(m_lua->state, sizeInBytes);
	}

	bool LuaEnvironment::toBool()
	{
		return lua_toboolean(m_lua->state, -1);
	}
	double LuaEnvironment::toNumber()
	{
		return luaL_checknumber(m_lua->state, -1);
	}
	std::string LuaEnvironment::toString()
	{
		return luaL_checkstring(m_lua->state, -1);
	}
	void* LuaEnvironment::toObject(const std::string &className)
	{
		return luaL_checkudata(m_lua->state, -1, className.c_str());
	}

	void LuaEnvironment::attachMetatable(const std::string &metatableName, int index)
	{
		luaL_getmetatable(m_lua->state, metatableName.c_str());
		int metaTable = lua_gettop(m_lua->state);

		// Associate self with C++ object
		lua_setfield(m_lua->state, metaTable, "__self");

		// Set metatable of the desired object
		lua_setmetatable(m_lua->state, index);
	}

	void LuaEnvironment::pushGlobalEnv()
	{
		lua_pushglobaltable(m_luaContext);
	}

	void LuaEnvironment::createClass(const std::string &className, const luaL_Reg *funcs)
	{
		luaL_newmetatable(m_luaContext, className.c_str());
		int metaTable = lua_gettop(m_luaContext);

		luaL_setfuncs(m_luaContext, funcs, 0);

		lua_pushvalue(m_luaContext, -1);
		lua_setfield(m_luaContext, metaTable, "__index");
	}
	void LuaEnvironment::registerFunctions(const std::vector<LuaFunction> &funcs)
	{
		/*lua_pushglobaltable(m_luaContext);
				const luaL_Reg audioFuncs[] =  ALL DIS JUST FOR GETTING THE LAYOUT
		{
			{ "PlaySound", PlaySound },
			{ "PlayStream", PlayStream },
			{ "RegisterAudio", RegisterAudio },
			{ NULL, NULL }
		};

		std::vector<luaL_Reg> luaFuncs;

		// Register sound functions at global scope
		lua_State *state = game->getLua().getRaw();
		lua_pushglobaltable(state);
		luaL_setfuncs (state, audioFuncs, 0);*/
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
		SAUROBYTE_ERROR_LOG("Lua error: ", lua_tostring(m_luaContext, -1));
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