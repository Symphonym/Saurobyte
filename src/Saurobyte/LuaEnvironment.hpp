
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

#ifndef SAUROBYTE_LUA_ENVIRONMENT_HPP
#define SAUROBYTE_LUA_ENVIRONMENT_HPP

#include <Saurobyte/ApiDefines.hpp>
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

	class SAUROBYTE_API LuaEnvironment
	{

	public:

		typedef std::function<int(LuaEnvironment&)> LuaFunctionPtr;
		typedef std::pair<std::string, LuaFunctionPtr> LuaFunction;

		LuaEnvironment();
		~LuaEnvironment();

		/**
		 * Pushes any amount of values onto the Lua stack
		 * @param  args Values to push onto the stack
		 * @return      How many values that were pushed onto the stack
		 */
		template<typename ...TArgs> int pushArgs(TArgs... args)
		{
			return 1 + pushArg(args...);
		};

		/**
		 * Pushes an arbitrary object onto the Lua stack with the specified metatable. This object will be garbage collected.
		 * @param newData   The data to push onto the Lua stack
		 * @param className The metatable associated with the data, the metatable doesn't have to exist.
		 */
		template<typename TType> void pushObject(TType newData, const std::string &className)
		{

			// Use placement new to allocate new LuaObject in Lua owned memory
			LuaObject<TType> *dataPtr = new (pushMemory(sizeof(LuaObject<TType>))) LuaObject<TType>(newData);

			attachMetatable(className, -1);
		}
		/**
		 * Pushes a nil value onto the Lua stack
		 */
		void pushNil();
		/**
		 * Pushes an empty table onto the Lua stack
		 */
		void pushTable();

		/**
		 * Executes t[k] = v, where v is the value at the top of the stack, and t just below the top.
		 * @param  key The key which the value will be mapped to in the table
		 * @return     Whether or not the writing operation succeeded
		 */
		bool tableWrite(int key);
		bool tableWrite(const std::string &key);

		/**
		 * Executes t[k], where k is the specified key, and t is at the top of the stack. The resulting value is inserted at the bottom of the stack.
		 * @param  key The key whose value will be read
		 * @return     Whether or not the reading operation succeeded
		 */
		bool tableRead(int key);
		bool tableRead(const std::string &key);

		/**
		 * Pops the value at the bottom of the stack and converts it to a boolean
		 * @return The boolean value at the bottom of the stack
		 */
		bool toBool();
		/**
		 * Pops the value at the bottom of the stack and converts it to a double
		 * @return The double value at the bottom of the stack
		 */
		double toNumber();
		/**
		 * Pops the value at the bottom of the stack and converts it to a string
		 * @return The string value at the bottom of the stack
		 */
		std::string toString();
		/**
		 * Pops the value at the bottom of the stack and converts it to a pointer (userdata)
		 * @return The pointer (userdata) value at the bottom of the stack
		 */
		template<typename TType> TType* toPointer()
		{
			TType* ptr = static_cast<TType*>(toPointer());
			return ptr;
		}
		/**
		 * Pops the value at the bottom of the stack and converts it to an object (userdata)
		 * @param  className The metatable that the object is using, to validate it
		 * @return           The object (userdata) at the bottom of hte stack
		 */
		template<typename TType> TType& toObject(const std::string &className)
		{
			LuaObject<TType> *data = static_cast<LuaObject<TType>*>(toObject(className));
			return data->data;
		};

		/**
		 * Pops the value at the top of the stack and sets it as a global in the Lua environment
		 * @param name The global name
		 */
		void writeGlobal(const std::string &name);
		/**
		 * Retrieves a global from the Lua environment and inserts it at the bottom of the stack, if it was found.
		 * @param  name The name of the global variable
		 * @return      Whether or not the global was found
		 */
		bool readGlobal(const std::string &name);

		/**
		 * Creates a Lua metatable to act as a class with the specified functions and name
		 * @param className The name of the metatable
		 * @param funcs     The functions that the metatable (class) provides
		 */
		void createClass(const std::string &className, const std::vector<LuaFunction> &funcs);
		/**
		 * Registers a global function into the Lua environment
		 * @param func The function to register
		 */
		void registerFunction(const LuaFunction &func);

		
		/**
		 * Runs a given Lua script
		 * @param  filePath Path to the script
		 * @return          Whether or not the script executed without errors
		 */
		bool runScript(const std::string &filePath);
		bool runScript(const std::string &filePath, int sandBoxID);

		int createSandbox(const std::vector<std::string> &disabledLuaFunctions);

		void reportError();

		std::size_t getMemoryUsage() const;

	private:

		struct LuaObjectBase
		{
			virtual ~LuaObjectBase() {};
		};
		template<typename TType> struct LuaObject : public LuaObjectBase
		{
			TType data;
			explicit LuaObject(TType newData) : data(newData) {};
		};

		std::unique_ptr<internal::LuaImpl> m_lua; 

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
			typename std::enable_if<std::is_convertible<TType, std::string>::value, int>::type pushArg(TType arg, TArgs... args)
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
		template<typename TType, typename ...TArgs> 
			typename std::enable_if<std::is_convertible<TType, LuaFunctionPtr>::value, int>::type pushArg(TType arg, TArgs... args)
		{
			pushFunction(arg);
			return 1 + pushArg(args...);
		};
		template<typename TType, typename ...TArgs> 
			typename std::enable_if<std::is_pointer<TType>::value && !std::is_convertible<TType, std::string>::value, int>::type pushArg(TType arg, TArgs... args)
		{
			pushPointer(arg);
			return 1 + pushArg(args...);
		};

		void pushBool(bool value);
		void pushNumber(double value);
		void pushString(const std::string &value);
		void pushFunction(const LuaFunctionPtr &function);
		void pushPointer(void *pointer);
		void* pushMemory(std::size_t sizeInBytes);

		void* toPointer();
		void* toObject(const std::string &className);


		void attachMetatable(const std::string &metatableName, int index);

	};
};

#endif