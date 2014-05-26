
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
#include <Saurobyte/NonCopyable.hpp>
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

	class LuaElement;
	class SAUROBYTE_API LuaEnvironment : public NonCopyable
	{

	public:

		typedef std::function<int(LuaEnvironment&)> LuaFunctionPtr;
		typedef std::pair<std::string, LuaFunctionPtr> LuaFunction;
		typedef std::function<void(LuaEnvironment&, int)> LuaLoopFunction;

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
			attachMetatable(className);
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
		 * Iterates the table at the top of the stack recursively, calling the handler for each element
		 * @param handler Function called for each element in the table, and its nested tables
		 */
		void iterateTable(LuaLoopFunction handler, LuaLoopFunction tableFinishHandler = LuaLoopFunction(nullptr));

		/**
		 * Reads function arguments within a function called by Lua, in the order they were passed
		 * @return The value of the function parameter converted to the specified type
		 */
		template<typename TType> TType readArg()
		{
			return readStackValue<TType>(1);
		};
		template<typename TType> TType& readArg(const std::string &className)
		{
			return toObject<TType>(className, 1);
		};

		/**
		 * Reads values from the top of the Lua stack
		 * @return The value at the top of the Lua stack converted to the specified type
		 */
		template<typename TType> TType readStack()
		{
			return readStackValue<TType>(-1);
		};
		template<typename TType> TType& readStack(const std::string &className)
		{
			return toObject<TType>(className, -1);
		};

		/**
		 * Pops the value at the top of the stack and sets it as a global in the Lua environment
		 * @param name The global name
		 */
		void writeGlobal(const std::string &name);
		/**
		 * Pops the value at the top of the stack and sets it as a global in the specified Lua environment
		 * @param name      The global name
		 * @param sandBoxID The sand box in which to register the global
		 */
		void writeGlobal(const std::string &name, int sandBoxID);
		/**
		 * Retrieves a global from the Lua environment and pushes it onto the stack, if it was found.
		 * @param  name The name of the global variable
		 * @return      Whether or not the global was found
		 */
		bool readGlobal(const std::string &name);
		/**
		 * Retrieves a global from the specified Lua environment and pushes it onto the stack, if it was found.
		 * @param name      The global name
		 * @param sandBoxID The sand box from which to read
		 */
		bool readGlobal(const std::string &name, int sandBoxID);

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
		 * Calls a Lua function, using values at the top of the stack as arguments
		 * @param  funcName      The function to call
		 * @param  argumentCount How many arguments to pop from the stack
		 * @return               The amount of return values that are pushed onto the stack
		 */
		int callFunction(const std::string &funcName, int argumentCount = 0);
		/**
		 * Calls a Lua function in the specified sand box, using values at the top of the stack as arguments
		 * @param  funcName      The function to call
		 * @param  argumentCount How many arguments to pop from the stack
		 * @param  sandBoxID     The sand box in which the function is located
		 * @return               The amount of return values that are pushed onto the stack
		 */
		int callFunction(const std::string &funcName, int argumentCount, int sandBoxID);

		/**
		 * Runs the specified Lua script
		 * @param  filePath Path to the script
		 * @return          Whether or not the script executed without errors
		 */
		bool runScript(const std::string &filePath);
		/**
		 * Runs a script within the specified Lua sand box
		 * @param  filePath  Path to the script
		 * @param  sandBoxID The sand box in which to run the script
		 * @return           Whether or not the script executed without errors
		 */
		bool runScript(const std::string &filePath, int sandBoxID);

		/**
		 * Creates a Lua sand box, which is a copy of the global Lua environment (in its current state)
		 * @param  disabledLuaFunctions The Lua modules (e.g os.execute) that will be disabled in the sand box environment
		 * @return                      Unique identifier of the sand box
		 */
		int createSandbox(const std::vector<std::string> &disabledLuaFunctions);


		/**
		 * Checks if the value at the top of the stack is a number
		 * @return Whether or not top stack value is a number
		 */
		bool isNumber() const;
		/**
		 * Checks if the value at the top of the stack is a string
		 * @return Whether or not top stack value is a string
		 */
		bool isString() const;
		/**
		 * Checks if the value at the top of the stack is a boolean
		 * @return Whether or not top stack value is a boolean
		 */
		bool isBool() const;
		/**
		 * Checks if the value at the top of the stack is a table
		 * @return Whether or not top stack value is a table
		 */
		bool isTable() const;
		/**
		 * Checks if the value at the top of the stack is a C++ object
		 * @return Whether or not top stack value is a C++ object
		 */
		bool isObject() const;

		/**
		 * Checks to see how much memory Lua is using
		 * @return Lua memory usage, in kilo bytes (kB)
		 */
		std::size_t getMemoryUsage() const;
		void pr();
	private:

		friend class LuaElement;

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
		template<typename TType, typename ...TArgs> 
			typename std::enable_if<std::is_convertible<TType, std::pair<TType, std::string> >::value, int>::type pushArg(TType arg, TArgs... args)
		{
			pushObject(arg.first, arg.second);
			return 1 + pushArg(args...);
		};

		template<typename TType> 
			typename std::enable_if<std::is_same<bool, TType>::value, TType>::type readStackValue(int index)
		{
			return toBool(index);
		};
		template<typename TType> 
			typename std::enable_if<std::is_convertible<TType, std::string>::value, TType>::type readStackValue(int index)
		{
			return toString(index);
		};
		template<typename TType> 
			typename std::enable_if<std::is_arithmetic<TType>::value && !std::is_same<bool, TType>::value, TType>::type readStackValue(int index)
		{
			return toNumber(index);
		};
		template<typename TType> 
			typename std::enable_if<std::is_convertible<TType, LuaFunctionPtr>::value, LuaFunctionPtr>::type readStackValue(int index)
		{
			return toObject<TType>("Saurobyte_LuaFunction", index);
		};

		void pushBool(bool value);
		void pushNumber(double value);
		void pushString(const std::string &value);
		void pushFunction(const LuaFunctionPtr &function);
		void pushPointer(void *pointer);
		void* pushMemory(std::size_t sizeInBytes);

		void* toObject(const std::string &className, int index);

		/**
		 * Pops the value at the index and converts it to a boolean
		 * @return The boolean value at the index
		 */
		bool toBool(int index);
		/**
		 * Pops the value at the index and converts it to a double
		 * @return The double value at the index
		 */
		double toNumber(int index);
		/**
		 * Pops the value at the index and converts it to a string
		 * @return The string value at the index
		 */
		std::string toString(int index);
		/**
		 * Pops the value at the index and converts it to an object (userdata)
		 * @param  className The metatable that the object is using, to validate it
		 * @return           The object (userdata) at the index
		 */
		template<typename TType> TType& toObject(const std::string &className, int index)
		{
			LuaObject<TType> *data = static_cast<LuaObject<TType>*>(toObject(className, index));
			return data->data;
		};

		/**
		 * Recursively iterates a table
		 * @param handler     The handler that will be called for each element in the hierarchy
		 * @param nestedLevel How deep the iterator is, starts and 0 and increments by 1 for each nested table it recurses into
		 */
		void iterateTableRecursive(LuaLoopFunction &handler, LuaLoopFunction &tableFinishHandler, int nestedLevel, int tableIndex);

		/**
		 * Creates and set the metatable for the value at the top of the stack, complete with gc
		 * @param metatableName The name of the metatable
		 */
		void attachMetatable(const std::string &metatableName);

		/**
		 * Reports Lua errors to the logger
		 */
		void reportError();
	};

	class SAUROBYTE_API LuaElement : public NonCopyable
	{
	public:

		template<typename TType> TType key()
		{
			pushKey();
			return m_env->readStack<TType>();
		};
		template<typename TType> TType value()
		{
			pushValue();
			return m_env->readStack<TType>();
		};

		LuaElement();
		~LuaElement();

		void pushValue();
		void pushKey();

	private:

		friend class LuaEnvironment;

		void unrefData();
		void refData(int keyRef, int valueRef);

		int m_keyID, m_valueID;
		LuaEnvironment *m_env;
	};
};

#endif