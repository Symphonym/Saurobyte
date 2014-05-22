
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
		int valueIndex = originalTableIndex - 1;

		for(std::size_t i = 0; i < tablePath.size(); i++)
		{
			std::string curTable = tablePath[i];

			// If this is the last segment of the path, it's the value
			if(i == tablePath.size()-1)
			{
				// Move the value to the top of the stack, so we can just traverse the table path and set all fields
				lua_pushvalue(m_lua->state, valueIndex);
				lua_remove(m_lua->state, valueIndex);
				
				// Traverse the table path and set all fields
				int index = 1;
				for(int i = lua_gettop(m_lua->state); i >= originalTableIndex; i--)
				{
					lua_setfield(m_lua->state, -2, tablePath[tablePath.size()-index].c_str());
					++index;
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
			else if(!lua_istable(m_lua->state, -1))
				return false;
		}

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
		std::vector<std::string> tablePath = Saurobyte::splitStr(key, '.');

		for(std::size_t i = 0; i < tablePath.size(); i++)
		{
			std::string curTable = tablePath[i];

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

		if(lua_istable(m_lua->state, -1) && lua_gettop(m_lua->state) >= 1)
		{
			lua_pushstring(m_lua->state, key.c_str());
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
			return 0;
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

		auto luaFunc = [] (lua_State *state) -> int
		{
			lua_getglobal(state, "SAUROBYTE_LUA_ENV");
			LuaEnvironment *luaEnv = static_cast<LuaEnvironment*>(lua_touserdata(state, -1));
			lua_pop(state, 1);

			LuaObject<LuaFunctionPtr>& funcPtr = *static_cast<LuaObject<LuaFunctionPtr>*>(
				lua_touserdata(state, lua_upvalueindex(1)));

			return funcPtr.data(*luaEnv);
		};

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

		//if(!getTableRecursive(name, true, sandBoxID))
		//	return;
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

		return tableRead(name);
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
			lua_pcall(m_lua->state, argumentCount, LUA_MULTRET, 0);
			int newTop = lua_gettop(m_lua->state) + 1;

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

		if(loadErrCode == LUA_ERRSYNTAX)
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

		for(std::size_t i = 0; i < disabledLuaFunctions.size(); i++)
		{
			std::string::size_type dotPos = disabledLuaFunctions[i].find('.');
			std::string moduleToDisable = disabledLuaFunctions[i];

			if(dotPos != std::string::npos)
			{
				std::string moduleTable = moduleToDisable.substr(0, dotPos);
				std::string moduleFunc = moduleToDisable.substr(dotPos+1, moduleToDisable.size());

				// Read lua module table
				if(tableRead(moduleTable))
				{
					// Overwrite function
					pushNil();
					tableWrite(moduleFunc);
					tableWrite(moduleTable);
				}
			}
			else
			{
				pushNil();
				tableWrite(moduleToDisable);
			}
		}

		// Store sand box in Lua registry and return identifier
		return luaL_ref(m_lua->state, LUA_REGISTRYINDEX);
	}

	bool LuaEnvironment::getTableRecursive(const std::string &nestedTable, bool createNonExistant, int sandBoxID)
	{
		if(sandBoxID == LUA_NOREF)
			lua_pushglobaltable(m_lua->state);
		else
			lua_rawgeti(m_lua->state, LUA_REGISTRYINDEX, sandBoxID);

		if(lua_isnil(m_lua->state, -1))
			return false;

		getTableRecursive(nestedTable, createNonExistant);

	}
	bool LuaEnvironment::getTableRecursive(const std::string &nestedTable, bool createNonExistant)
	{

		std::vector<std::string> tablePath = Saurobyte::splitStr(nestedTable, '.');

		SAUROBYTE_INFO_LOG("START");
		for(std::size_t i = 0; i < tablePath.size(); i++)
		{
			std::string curPath = tablePath[i];
			SAUROBYTE_INFO_LOG("CUr ", curPath);
			if(tableRead(curPath))
				tableWrite(curPath);
			else
			{
				SAUROBYTE_INFO_LOG("Not found ", curPath);
				pushTable();
				tableWrite(curPath);
				tableRead(curPath); // Push it back on the stack
			}

			if(!lua_istable(m_lua->state, -1))
			{
				lua_pop(m_lua->state, 1);
				return true;
			}


			// Remove previous table
			lua_remove(m_lua->state, -2);
		}



		return true;

		std::string::size_type dotPos = nestedTable.find('.');

		// End of table path
		if(dotPos == std::string::npos)
			dotPos = nestedTable.size()-1;

		std::string currentTable = nestedTable.substr(0, dotPos);
		std::string nextPath = nestedTable.substr(dotPos+1, nestedTable.size());
		printf("\nCurtab %s, next %s\n", currentTable.c_str(), nextPath.c_str());

		// Attempts to read the current table from the previous table
		bool tableExists = tableRead(currentTable);

		if(!lua_istable(m_lua->state, -1))
		{
			lua_pop(m_lua->state, 1);
			return true;
		}
		else if(!tableExists)
		{
			if(createNonExistant)
			{
				pushTable();
				tableWrite(currentTable); // Write the table to the previous table
				return getTableRecursive(nextPath, createNonExistant);
			}
			else
				return false;
		}

		if(currentTable.empty() || nextPath.empty())
			return true;

		return getTableRecursive(nextPath, createNonExistant);
		/*lua_getfield(m_lua->state, -1, currentTable.c_str());
		printf("\nCurtab %s, next %s\n", currentTable.c_str(), nextPath.c_str());

		if(lua_isnil(m_lua->state, -1))
		{
			lua_pop(m_lua->state, 1); // Pop nil

			// Create table if that's specified
			if(createNonExistant)
			{
				pushTable();
				lua_setfield(m_lua->state, -1, currentTable.c_str());
				lua_getfield(m_lua->state, -1, currentTable.c_str());
			}
			else
			{
				lua_pop(m_lua->state, 1); // Pop table
				return true;
			}
		}

		// If we come across a non-table value, pop it and return the previous table
		if(!lua_istable(m_lua->state, -1))
		{
			lua_remove(m_lua->state, -1);
			return true;
		}

		// Recurse into next table
		else
		{
			printf("\nRecursing into %s, was in %s\n", nextPath.c_str(), currentTable.c_str());
			return getTableRecursive(nextPath, createNonExistant);
		}*/
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