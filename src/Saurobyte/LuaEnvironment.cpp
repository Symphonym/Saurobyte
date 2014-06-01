
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
#include <Saurobyte/Util.hpp>
#include <Lua/lua.hpp>

namespace Saurobyte
{


	LuaEnvironment::LuaEnvironment()
	{

		m_lua = std::unique_ptr<internal::LuaImpl>(new internal::LuaImpl());

		lua_pushlightuserdata(m_lua->state, this);
		lua_setglobal(m_lua->state, "SAUROBYTE_LUA_ENV");

		// Setup function objects
	/*	auto luaFunc = [] (lua_State *state) -> int
		{
			lua_getglobal(state, "SAUROBYTE_LUA_ENV");
			LuaEnvironment *luaEnv = static_cast<LuaEnvironment*>(lua_touserdata(state, -1));
			lua_pop(state, 1);


			LuaObject<LuaFunctionPtr>& funcPtr = *static_cast<LuaObject<LuaFunctionPtr>*>(lua_touserdata(state, 1));
			lua_remove(state, 1);

			return funcPtr.data(*luaEnv);
		};

		//lua_pushlightuserdata(m_lua->state, this);
		luaL_newmetatable(m_lua->state, "Saurobyte_LuaFunction");
		int metatable = lua_gettop(m_lua->state);

		lua_pushcfunction(m_lua->state, luaFunc);
		lua_setfield(m_lua->state, metatable, "__call");*/

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
		auto luaFunc = [] (lua_State *state) -> int
		{
			lua_getglobal(state, "SAUROBYTE_LUA_ENV");
			LuaEnvironment *luaEnv = static_cast<LuaEnvironment*>(lua_touserdata(state, -1));
			lua_pop(state, 1);

			LuaObject<LuaFunctionPtr>& funcPtr = *static_cast<LuaObject<LuaFunctionPtr>*>(
				lua_touserdata(state, lua_upvalueindex(1)));

			return funcPtr.data(*luaEnv);
		};

		pushObject<LuaFunctionPtr>(function, "Saurobyte_LuaFunction");
		lua_pushcclosure(m_lua->state, luaFunc, 1);
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

	bool LuaEnvironment::tableWrite(int key)
	{
		if(lua_istable(m_lua->state, -2) && lua_gettop(m_lua->state) >= 2)
		{
			lua_pushnumber(m_lua->state, key);

			// Move the key in front of the value associated with the key
			lua_insert(m_lua->state, lua_gettop(m_lua->state)-1);
			lua_settable(m_lua->state, -3);
			return true;
		}
		else
			return false;
	}
	bool LuaEnvironment::tableWrite(const std::string &key)
	{		
		std::vector<std::string> tablePath = Saurobyte::splitStr(key, '.');

		// Shuffle the value with the table, allowing for easier traversing of nested tables
		lua_insert(m_lua->state, lua_gettop(m_lua->state)-1);

		int originalTableIndex = lua_gettop(m_lua->state);
		int valueIndex = lua_gettop(m_lua->state)-1;

		for(std::size_t i = 0; i < tablePath.size(); i++)
		{
			std::string curTable = tablePath[i];

			// End of path
			if(i == tablePath.size()-1)
			{
				// Shift the value back to the top
				lua_pushvalue(m_lua->state, valueIndex);
				lua_remove(m_lua->state, valueIndex);

				// Traverse the table path and set all fields
				int index = 1;
				for(int i = lua_gettop(m_lua->state); i > valueIndex; i--)
				{
					// Corner case with userdata and metatables
					if(lua_isuserdata(m_lua->state, -2))
						lua_setmetatable(m_lua->state, -2);
					else
					{
						lua_setfield(m_lua->state, -2, tablePath[tablePath.size()-index].c_str());
						++index;
					}
				}
				return true;
			}

			lua_getfield(m_lua->state, -1, curTable.c_str());

			if(lua_isnil(m_lua->state, -1))
			{
				// Pop nil value and push another table for the path
				lua_pop(m_lua->state, 1);
				pushTable();
			}
			else if(lua_isuserdata(m_lua->state, -1))
				lua_getmetatable(m_lua->state, -1);
			
			if(!lua_istable(m_lua->state, -1))
			{
				// A value in the path wasen't a table, pop it and return write failure
				lua_pop(m_lua->state, 1);
				return false;
			}
		}

		// This shouldn't be reachd
		return false;
	}
	bool LuaEnvironment::tableRead(int key)
	{
		if(lua_istable(m_lua->state, -1) && lua_gettop(m_lua->state) >= 1)
		{
			lua_pushnumber(m_lua->state, key);
			lua_gettable(m_lua->state, -2);

			// The table doesn't contain an entry by the specified key, pop nil value
			if(lua_isnil(m_lua->state, -1))
			{
				lua_pop(m_lua->state, 1);
				return false;
			}
			else
				return true;
		}
		else
			return false;
	}
	bool LuaEnvironment::tableRead(const std::string &key)
	{
		if(!lua_istable(m_lua->state, -1) || lua_gettop(m_lua->state) < 1)
			return false;

		std::vector<std::string> tablePath = Saurobyte::splitStr(key, '.');

		for(std::size_t i = 0; i < tablePath.size(); i++)
		{
			std::string curTable = tablePath[i];

			if(lua_isuserdata(m_lua->state, -1))
			{
				lua_getmetatable(m_lua->state, -1); // Traverse into metatables of userdata
				lua_remove(m_lua->state, -2); // Remove the userdata
			}

			lua_getfield(m_lua->state, -1, curTable.c_str());

			// The value was not found
			if(lua_isnil(m_lua->state, -1))
			{
				lua_pop(m_lua->state, 1);
				return false;
			}

			// Remove old nested tables, but not the original one (top level)
			if(i > 0)
				lua_remove(m_lua->state, -2);
		}
		return true;
	}

	void LuaEnvironment::iterateTable(LuaLoopFunction handler, LuaLoopFunction tableFinishHandler)
	{
		int tableIndex = lua_gettop(m_lua->state);
		if(lua_istable(m_lua->state, -1))
		{
			pushNil();
			iterateTableRecursive(handler, tableFinishHandler, 1, tableIndex);
		}
	}

	void LuaEnvironment::iterateTableRecursive(LuaLoopFunction &handler, LuaLoopFunction &tableFinishHandler, int nestedLevel, int tableIndex)
	{
		while(lua_next(m_lua->state, tableIndex))
		{
			bool isValueTable = lua_istable(m_lua->state, -1);
			int valueID = luaL_ref(m_lua->state, LUA_REGISTRYINDEX);
			int keyID = luaL_ref(m_lua->state, LUA_REGISTRYINDEX);

			// This is the stack size we want to maintain
			int prevStackSize = lua_gettop(m_lua->state);

			lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, valueID);
			lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, keyID);


			// Call handler and cleanup "leftovers" (on the stack) from it
			handler(*this, nestedLevel);
			lua_pop(m_lua->state, lua_gettop(m_lua->state) - prevStackSize);

			// Recurse into nested tables
			if(isValueTable)
			{

				lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, valueID);
				pushNil();
				iterateTableRecursive(handler, tableFinishHandler, nestedLevel+1, lua_gettop(m_lua->state) -1);

				// Pop the table that was recursed
				lua_pop(m_lua->state, 1);

				// Calls optional handler at end of table recursion
				if(tableFinishHandler != nullptr)
				{
					lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, valueID);
					lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, keyID);
					tableFinishHandler(*this, nestedLevel);
				}

				// Make sure stack is not contaminated by the user
				lua_pop(m_lua->state, lua_gettop(m_lua->state) - prevStackSize);
			}

			// Push key for next iteration and unref data for this element
			lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, keyID);
			luaL_unref(m_lua->state, LUA_REGISTRYINDEX, valueID);
			luaL_unref(m_lua->state, LUA_REGISTRYINDEX, keyID);
		}
	}

	bool LuaEnvironment::toBool(int index)
	{
		bool value = lua_toboolean(m_lua->state, index);
		lua_remove(m_lua->state, index);
		return value;
	}
	double LuaEnvironment::toNumber(int index)
	{
		double value = luaL_checknumber(m_lua->state, index);
		lua_remove(m_lua->state, index);
		return value;
	}
	std::string LuaEnvironment::toString(int index)
	{
		std::string value = luaL_checkstring(m_lua->state, index);
		lua_remove(m_lua->state, index);
		return value;
	}
	void* LuaEnvironment::toObject(const std::string &className, int index)
	{
		void *value = luaL_checkudata(m_lua->state, index, className.c_str());
		lua_remove(m_lua->state, index);
		return value;
	}

	void LuaEnvironment::attachMetatable(const std::string &metatableName)
	{
		luaL_newmetatable(m_lua->state, metatableName.c_str());
		int metaTable = lua_gettop(m_lua->state);

		// Associate self with the C++ object
		lua_pushvalue(m_lua->state, metaTable);
		lua_setfield(m_lua->state, metaTable, "__self");

		// Associate indexing with metatable as well (making the userdata act as a table, so you can store data in it)
		lua_pushvalue(m_lua->state, metaTable);
		lua_pushvalue(m_lua->state, metaTable);
		lua_setfield(m_lua->state, metaTable, "__index");
		lua_setfield(m_lua->state, metaTable, "__newindex");

		// Garbage collecting
		lua_pushcfunction(m_lua->state, 
		[] (lua_State *state) -> int
		{
			// Call virtual destructor of base class, this allows a very generic gc function
			LuaObjectBase* obj = static_cast<LuaObjectBase*>(lua_touserdata(state, 1));
			obj->~LuaObjectBase();
			return 0;
		});
		lua_setfield(m_lua->state, metaTable, "__gc");	


		// Set metatable of the desired object
		lua_setmetatable(m_lua->state, -2);



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

		// Pop metatable off stack
		lua_pop(m_lua->state, 1);
	}

	void LuaEnvironment::registerFunction(const LuaFunction &func)
	{
		pushFunction(func.second);
		writeGlobal(func.first);

	}
	void LuaEnvironment::writeGlobal(const std::string &name)
	{
		writeGlobal(name, LUA_NOREF);
	}
	void LuaEnvironment::writeGlobal(const std::string &name, int sandBoxID)
	{
		if(lua_gettop(m_lua->state) < 1)
			return;

		if(sandBoxID == LUA_NOREF)
			lua_pushglobaltable(m_lua->state);
		else
			lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, sandBoxID);

		if(lua_isnil(m_lua->state, -1))
		{
			lua_pop(m_lua->state, 1);
			return;
		}

		// Move the sandbox/global table just below the top, so that the global value is at the top
		lua_insert(m_lua->state, lua_gettop(m_lua->state) - 1);
		tableWrite(name);

		// Pop env table
		lua_pop(m_lua->state, 1);
	}
	bool LuaEnvironment::readGlobal(const std::string &name)
	{
		return readGlobal(name, LUA_NOREF);
	}
	bool LuaEnvironment::readGlobal(const std::string &name, int sandBoxID)
	{
		if(sandBoxID == LUA_NOREF)
			lua_pushglobaltable(m_lua->state);
		else
			lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, sandBoxID);

		if(lua_isnil(m_lua->state, -1))
		{
			lua_pop(m_lua->state, 1);
			return false;
		}

		bool returnValue = tableRead(name);
		lua_remove(m_lua->state, returnValue ? -2 : -1); // Remove env table
		return returnValue;
	}

	int LuaEnvironment::callFunction(const std::string &funcName, int argumentCount)
	{
		return callFunction(funcName, argumentCount, LUA_NOREF);
	}
	int LuaEnvironment::callFunction(const std::string &funcName, int argumentCount, int sandBoxID)
	{
		if(readGlobal(funcName, sandBoxID))
		{
			int oldTop = lua_gettop(m_lua->state) - argumentCount;
			int errCode = lua_pcall(m_lua->state, argumentCount, LUA_MULTRET, 0);
			int newTop = lua_gettop(m_lua->state) + 1;

			if(errCode != LUA_OK)
				reportError();

			return newTop - oldTop;
		}
		else
			return 0;
	}

	bool LuaEnvironment::runScript(const std::string &filePath)
	{
		return runScript(filePath, LUA_NOREF);		
	}
	bool LuaEnvironment::runScript(const std::string &filePath, int sandBoxID)
	{
		int loadErrCode = luaL_loadfile(m_lua->state, filePath.c_str());

		if(loadErrCode != LUA_OK)
		{
			reportError();
			return false;
		}
		else
		{
			// Set custom environment
			if(sandBoxID != LUA_NOREF)
			{
				lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, sandBoxID);
				lua_setupvalue(m_lua->state, -2, 1);
			}

			// Run Lua chunk
			int runErrCode = lua_pcall(m_lua->state, 0, 0, 0);
			if(runErrCode != LUA_OK)
			{
				reportError();
				return false;
			}

			return true;
		}
	}

	int LuaEnvironment::createSandbox(const std::vector<std::string> &disabledLuaFunctions)
	{
		// Create new table to hold the sand box environment
		lua_newtable(m_lua->state);

		// Associate existing global table with the new sand box
		lua_getglobal(m_lua->state, "_G");
		lua_setfield(m_lua->state, -2, "__index");

		// Store sand box in Lua registry
		int sandBox = luaL_ref(m_lua->state, LUA_REGISTRYINDEX);
		
		for(std::size_t i = 0; i < disabledLuaFunctions.size(); i++)
		{
			std::string curFunc = disabledLuaFunctions[i];

			if(readGlobal(curFunc, sandBox))
			{
				pushNil();
				writeGlobal(curFunc, sandBox);
			}
		}

		return sandBox;
	}

	void LuaEnvironment::reportError()
	{
		if(lua_isstring(m_lua->state, -1))
			SAUROBYTE_ERROR_LOG("Lua error: ", readStack<std::string>());
	}

	bool LuaEnvironment::isNumber() const
	{
		return lua_type(m_lua->state, -1) == LUA_TNUMBER; // Due to isnumber returning true for certain strings
	}
	bool LuaEnvironment::isString() const
	{
		return lua_type(m_lua->state, -1) == LUA_TSTRING; // Due to isstring returning true for certain numbers
	}
	bool LuaEnvironment::isBool() const
	{
		return lua_isboolean(m_lua->state, -1);
	}
	bool LuaEnvironment::isTable() const
	{
		return lua_istable(m_lua->state, -1);
	}
	bool LuaEnvironment::isObject() const
	{
		return lua_isuserdata(m_lua->state, -1);
	}

	std::size_t LuaEnvironment::getMemoryUsage() const
	{
		return lua_gc(m_lua->state, LUA_GCCOUNT, 0);
	}

};