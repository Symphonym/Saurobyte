
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


#ifndef SAUROBYTE_COMPONENT_HPP
#define SAUROBYTE_COMPONENT_HPP

#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <string>
#include <unordered_map>



namespace Saurobyte
{

	/*
		BaseComponent

		Used primarily to give template component classes a common
		base which provides most of the type specific data.

	*/
	class BaseComponent
	{

	private:

		const TypeID m_typeID;

	protected:

		BaseComponent(TypeID typeID) 
			:
			m_typeID(typeID)
		{};


	public:


		virtual ~BaseComponent() {};

		// Functions called by Lua if a LuaComponent is attached
		virtual void onLuaSet(const std::string& valueName, LuaEnvironment &env) {};
		virtual int onLuaGet(const std::string& valueName, LuaEnvironment &env) {};

		// Components must provide a name for Lua usage
		virtual std::string getName() const = 0;

		TypeID getTypeID() const;

		// Cloning function that must be overridden by deriving classes
		virtual BaseComponent* clone()  = 0;
	};

	template<typename TType> class Component : public BaseComponent
	{

	public:

		// Component names are optional, but they won't be accessable through Lua then
		Component() 
			:
			BaseComponent(TypeIdGrabber::getUniqueTypeID<TType>())
		{};
		virtual ~Component() {};

		// Override cloning
		virtual BaseComponent* clone() 
		{
			// CRTP - Curiously Recurring Template Pattern
			return new TType(static_cast<TType const&>(*this));
		};

	};

};

#endif