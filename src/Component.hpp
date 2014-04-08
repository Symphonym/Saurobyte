#ifndef JL_COMPONENT_HPP
#define JL_COMPONENT_HPP

#include <string>
#include <unordered_map>
#include <Lua/lua.hpp>
#include "IdentifierTypes.hpp"


namespace jl
{

	/*
		BaseComponent

		Used primarily to give template component classes a common
		base which provides most of the type specific data.

	*/
	class BaseComponent
	{

	protected:

		BaseComponent(TypeID typeID) 
			:
			typeID(typeID)
		{};

	public:

		const TypeID typeID;

		virtual ~BaseComponent() {};

		// Functions called by Lua if a LuaComponent is attached
		virtual void onLuaSet(const std::string& valueName, lua_State *state) {};
		virtual int onLuaGet(const std::string& valueName, lua_State *state) {};

		// Components must provide a name for Lua usage
		virtual std::string getName() const = 0;

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