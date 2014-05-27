
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

#include <Saurobyte/LuaConfig.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/Logger.hpp>
#include <Lua/lua.hpp>
#include <fstream>

namespace Saurobyte
{
	LuaConfig::LuaConfig(LuaEnvironment &env)
		:
		m_env(env),
		m_luaSandbox(LUA_NOREF)
	{
		m_luaSandbox = m_env.createSandbox(
		{
			"os", "io", "package", "coroutine"
		});

		// Where all values are stored
		m_env.pushTable();
		m_env.writeGlobal("SauroConf");
	}

	bool LuaConfig::load(const std::string &filePath)
	{
		return m_env.runScript(filePath, m_luaSandbox);
	}
	void LuaConfig::save(const std::string &filePath)
	{
		std::ofstream writer(filePath, std::ofstream::trunc);

		if(m_env.readGlobal("SauroConf", m_luaSandbox))
		{
			writer << "SauroConf =" << std::endl << "{" << std::endl;

			m_env.iterateTable(
				[&writer, &filePath] (LuaEnvironment &env, int nestedLevel)
				{
					// Create indention based on how nested the iteration is
					std::string stringIndent;
					stringIndent.append(nestedLevel, '\t');

					// Handle key values
					if(env.isString())
						writer << stringIndent << env.readStack<std::string>() << " = ";
					else if (env.isNumber())
						writer << stringIndent << "[" << env.readStack<int>() << "] = ";
					else
					{
						SAUROBYTE_WARNING_LOG("Saving config file '", filePath, "' failed due to invalid key value. File will be corrupt.");
						return;
					}

					// Handle values
					if(env.isTable())
						writer << std::endl << stringIndent << "{" << std::endl;
					else if(env.isString())
						writer << "\"" << env.readStack<std::string>() << "\"," << std::endl;
					else if(env.isNumber())
						writer << env.readStack<double>() << "," << std::endl;
					else if(env.isBool())
						writer << std::boolalpha << env.readStack<bool>() << "," << std::endl;
					else
						writer << "nil -- An unsupported value was written" << std::endl;

				},
				[&writer] (LuaEnvironment &env, int nestedLevel)
				{
					// Create indention based on how nested the iteration is
					std::string stringIndent;
					stringIndent.append(nestedLevel, '\t');

					writer << stringIndent << "}," << std::endl;
				}	
			);

			writer << "}";
		}
	}

	int LuaConfig::readInt(const std::string &name, int defaultValue)
	{
		if(m_env.readGlobal(name, m_luaSandbox))
			return m_env.readStack<int>();
		else
			return defaultValue;
	} 
	double LuaConfig::readDouble(const std::string &name, double defaultValue)
	{
		if(m_env.readGlobal(name, m_luaSandbox))
			return m_env.readStack<double>();
		else
			return defaultValue;
	}
	std::string LuaConfig::readString(const std::string &name, const std::string &defaultValue)
	{
		if(m_env.readGlobal(name, m_luaSandbox))
			return m_env.readStack<std::string>();
		else
			return defaultValue;
	}
	bool LuaConfig::readBool(const std::string &name, bool defaultValue)
	{
		if(m_env.readGlobal(name, m_luaSandbox))
			return m_env.readStack<bool>();
		else
			return defaultValue;
	}

	
	void LuaConfig::writeInt(const std::string &name, int value)
	{
		m_env.pushArgs(value);
		m_env.writeGlobal(name, m_luaSandbox);
	}
	void LuaConfig::writeDouble(const std::string &name, double value)
	{
		m_env.pushArgs(value);
		m_env.writeGlobal(name, m_luaSandbox);
	}
	void LuaConfig::writeString(const std::string &name, const std::string &value)
	{
		m_env.pushArgs(value);
		m_env.writeGlobal(name, m_luaSandbox);
	}
	void LuaConfig::writeBool(const std::string &name, bool value)
	{
		m_env.pushArgs(value);
		m_env.writeGlobal(name, m_luaSandbox);
	}
};