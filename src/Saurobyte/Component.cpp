#include <Saurobyte/Component.hpp>

namespace Saurobyte
{
	TypeID BaseComponent::getTypeID() const
	{
		return m_typeID;
	}
};