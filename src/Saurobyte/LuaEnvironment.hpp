
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

	class LuaEnvironment
	{

	public:

		typedef std::function<int(LuaEnvironment&)> LuaFunctionPtr;
		typedef std::pair<std::string, LuaFunctionPtr> LuaFunction;

		LuaEnvironment();
		~LuaEnvironment();

		template<typename ...TArgs> int pushArgs(TArgs... args)
		{
			return 1 + pushArg(args...);
		};

		template<typename TType> void pushObject(TType newData, const std::string &className)
		{

			// Use placement new to allocate new LuaObject in Lua owned memory
			LuaObject<TType> *dataPtr = new (pushMemory(sizeof(LuaObject<TType>))) LuaObject<TType>(newData);

			attachMetatable(className, -1);
		}
		void pushNil();
		void pushTable();

		bool tableInsert(int key);
		bool tableInsert(const std::string &key);

		bool toBool();
		double toNumber();
		std::string toString();
		template<typename TType> TType* toPointer()
		{
			TType* ptr = static_cast<TType*>(toPointer());
			return ptr;
		}
		template<typename TType> TType& toObject(const std::string &className)
		{
			LuaObject<TType> *data = static_cast<LuaObject<TType>*>(toObject(className));
			return data->data;
		};


		void writeGlobal(const std::string &name);
		bool readGlobal(const std::string &name);

		// Registers a number of functions in a metatable with the name specified by 'className'
		void createClass(const std::string &className, const std::vector<LuaFunction> &funcs);
		void registerFunction(const LuaFunction &func);

		// Runs a given script and returns false if errors occurred
		bool runScript(const std::string &filePath);
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

						// Stop condition for arg pusher
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
			typename std::enable_if<std::is_pointer<TType>::value, int>::type pushArg(TType arg, TArgs... args)
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