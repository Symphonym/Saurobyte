
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

#ifndef SAUROBYTE_LUA_CONFIG_HPP
#define SAUROBYTE_LUA_CONFIG_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <string>

namespace Saurobyte
{
	class LuaEnvironment;
	class SAUROBYTE_API LuaConfig
	{
	public:

		explicit LuaConfig(LuaEnvironment &env);

		/**
		 * Loads the specified configuration Lua file
		 * @param  filePath Path to the Lua file
		 * @return          If the file was loaded succesfully
		 */
		bool load(const std::string &filePath);
		/**
		 * Saves all values in the SauroConf table to the specified Lua file (truncating previous data in the file)
		 * @param filePath Path to the Lua file, non existant file will be created
		 */
		void save(const std::string &filePath);

		/**
		 * Reads the specified integer from the Lua config environment
		 * @param  name         Name of the integer, path to nested tables are supported
		 * @param  defaultValue The value that is returned if the config value was not found
		 * @return              The value
		 */
		int readInt(const std::string &name, int defaultValue); 
		/**
		 * Reads the specified double from the Lua config environment
		 * @param  name         Name of the double, path to nested tables are supported
		 * @param  defaultValue The value that is returned if the config value was not found
		 * @return              The value
		 */
		double readDouble(const std::string &name, double defaultValue);
		/**
		 * Reads the specified string from the Lua config environment
		 * @param  name         Name of the string, path to nested tables are supported
		 * @param  defaultValue The value that is returned if the config value was not found
		 * @return              The value
		 */
		std::string readString(const std::string &name, const std::string &defaultValue);
		/**
		 * Reads the specified boolean from the Lua config environment
		 * @param  name         Name of the boolean, path to nested tables are supported
		 * @param  defaultValue The value that is returned if the config value was not found
		 * @return              The value
		 */
		bool readBool(const std::string &name, bool defaultValue);

		
		/**
		 * Writes the specified integer by the specifed identifier. Paths to nested tables are supported.
		 * @param name  Name to store the value by, note that only variables written to the SauroConf table will be saved.
		 * @param value The value to write
		 */
		void writeInt(const std::string &name, int value);
		/**
		 * Writes the specified double by the specifed identifier. Paths to nested tables are supported.
		 * @param name  Name to store the value by, note that only variables written to the SauroConf table will be saved.
		 * @param value The value to write
		 */
		void writeDouble(const std::string &name, double value);
		/**
		 * Writes the specified string by the specifed identifier. Paths to nested tables are supported.
		 * @param name  Name to store the value by, note that only variables written to the SauroConf table will be saved.
		 * @param value The value to write
		 */
		void writeString(const std::string &name, const std::string &value);
		/**
		 * Writes the specified bool by the specifed identifier. Paths to nested tables are supported.
		 * @param name  Name to store the value by, note that only variables written to the SauroConf table will be saved.
		 * @param value The value to write
		 */
		void writeBool(const std::string &name, bool value);

	private:

		LuaEnvironment &m_env;
		int m_luaSandbox;

	};
}


#endif