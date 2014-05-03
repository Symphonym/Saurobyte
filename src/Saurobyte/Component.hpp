#ifndef JL_COMPONENT_HPP
#define JL_COMPONENT_HPP

#include <string>
#include <unordered_map>
#include <Lua/lua.hpp>
#include <Saurobyte/IdentifierTypes.hpp>


namespace jl
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
		virtual void onLuaSet(const std::string& valueName, lua_State *state) {};
		virtual int onLuaGet(const std::string& valueName, lua_State *state) {};

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