
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


#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/LuaImpl.hpp>
#include <Lua/lua.hpp>

namespace Saurobyte
{


	LuaEnvironment::LuaEnvironment()
	{

		m_lua = std::unique_ptr<internal::LuaImpl>(new internal::LuaImpl());

		lua_pushlightuserdata(m_lua->state, this);
		lua_setglobal(m_lua->state, "SAUROBYTE_LUA_ENV");

		// Setup function objects
		auto luaFunc = [] (lua_State *state) -> int
		{
			lua_getglobal(state, "SAUROBYTE_LUA_ENV");
			LuaEnvironment *luaEnv = static_cast<LuaEnvironment*>(lua_touserdata(state, -1));
			lua_pop(state, 1);


			LuaObject<LuaFunctionPtr>& funcPtr = *static_cast<LuaObject<LuaFunctionPtr>*>(lua_touserdata(state, 1));
			lua_remove(state, 1);

			funcPtr.data(*luaEnv);
		};

		//lua_pushlightuserdata(m_lua->state, this);
		luaL_newmetatable(m_lua->state, "Saurobyte_LuaFunction");
		int metatable = lua_gettop(m_lua->state);

		lua_pushcfunction(m_lua->state, luaFunc);
		lua_setfield(m_lua->state, metatable, "__call");

	}
	LuaEnvironment::~LuaEnvironment()
	{

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
	void LuaEnvironment::pushFunction(const LuaFunctionPtr &function)
	{
		pushObject<LuaFunctionPtr>(function, "Saurobyte_LuaFunction");
	}
	void LuaEnvironment::pushPointer(void *pointer)
	{
		lua_pushlightuserdata(m_lua->state, pointer);
	}
	void* LuaEnvironment::pushMemory(std::size_t sizeInBytes)
	{
		return lua_newuserdata(m_lua->state, sizeInBytes);
	}
	void LuaEnvironment::pushNil()
	{
		lua_pushnil(m_lua->state);
	}
	void LuaEnvironment::pushTable()
	{
		lua_newtable(m_lua->state);
	}

	bool LuaEnvironment::tableInsert(int key)
	{
		if(lua_istable(m_lua->state, 1))
		{
			lua_push
			lua_settable(m_lua->state, 1)
		}
		else
			return false;
	}
	bool LuaEnvironment::tableInsert(const std::string &key)
	{

	}

	bool LuaEnvironment::toBool()
	{
		bool value = lua_toboolean(m_lua->state, 1);
		lua_pop(m_lua->state, 1);
		return value;
	}
	double LuaEnvironment::toNumber()
	{
		double value = luaL_checknumber(m_lua->state, 1);
		lua_pop(m_lua->state, 1);
		return value;
	}
	std::string LuaEnvironment::toString()
	{
		std::string value = luaL_checkstring(m_lua->state, 1);
		lua_pop(m_lua->state, 1);
		return value;
	}
	void* LuaEnvironment::toPointer()
	{
		void *value = lua_touserdata(m_lua->state, 1);
		lua_pop(m_lua->state, 1);
		return value;
	}
	void* LuaEnvironment::toObject(const std::string &className)
	{
		void *value = luaL_checkudata(m_lua->state, 1, className.c_str());
		lua_pop(m_lua->state, 1);
		return value;
	}

	void LuaEnvironment::attachMetatable(const std::string &metatableName, int index)
	{
		if(index < 0)
			index = lua_gettop(m_lua->state) + 1 + index;

		luaL_newmetatable(m_lua->state, metatableName.c_str());
		int metaTable = lua_gettop(m_lua->state);

		// Associate self with the C++ object
		lua_pushvalue(m_lua->state, index);
		lua_setfield(m_lua->state, metaTable, "__self");

		// Garbage collecting
		lua_pushcfunction(m_lua->state, 
		[] (lua_State *state) -> int
		{
			// Call virtual destructor of base class, this allows a very generic gc function
			LuaObjectBase* obj = static_cast<LuaObjectBase*>(lua_touserdata(state, 1));
			obj->~LuaObjectBase();
		});
		lua_setfield(m_lua->state, metaTable, "__gc");	


		// Set metatable of the desired object
		lua_setmetatable(m_lua->state, index);



	}

	void LuaEnvironment::createClass(const std::string &className, const std::vector<LuaFunction> &funcs)
	{
		luaL_newmetatable(m_lua->state, className.c_str());
		int metaTable = lua_gettop(m_lua->state);

		for(std::size_t i = 0; i < funcs.size(); i++)
		{
			
			//lua_pushcfunction(m_lua->state, luaFunc);
			pushFunction(funcs[i].second);
			lua_setfield(m_lua->state, metaTable, funcs[i].first.c_str());
		}

		lua_pushvalue(m_lua->state, metaTable);
		lua_setfield(m_lua->state, metaTable, "__index");
	}

	void LuaEnvironment::registerFunction(const LuaFunction &func)
	{

		pushObject<LuaFunctionPtr>(func.second, "Saurobyte_LuaFunction");
		writeGlobal(func.first);

	}
	void LuaEnvironment::writeGlobal(const std::string &name)
	{
		lua_setglobal(m_lua->state, name.c_str());
	}
	bool LuaEnvironment::readGlobal(const std::string &name)
	{
		lua_getglobal(m_lua->state, name.c_str());

		if(lua_isnil(m_lua->state, -1))
		{
			lua_pop(m_lua->state, 1);
			return false;
		}
		else
		{
			lua_insert(m_lua->state, 1);
			return true;
		}
	}


	bool LuaEnvironment::runScript(const std::string &filePath)
	{
		if(luaL_dofile(m_lua->state, filePath.c_str()))
		{
			reportError();
			return false;
		}
		else
			return true;
	}
	void LuaEnvironment::reportError()
	{
		SAUROBYTE_ERROR_LOG("Lua error: ", lua_tostring(m_lua->state, -1));
	}

	std::size_t LuaEnvironment::getMemoryUsage() const
	{
		return lua_gc(m_lua->state, LUA_GCCOUNT, 0);
	}


};