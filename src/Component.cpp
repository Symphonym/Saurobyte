#include "Component.hpp"

namespace jl
{
	TypeID BaseComponent::getTypeID() const
	{
		return m_typeID;
	}
};