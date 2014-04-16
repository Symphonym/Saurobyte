#include "BoundingBox.hpp"

namespace jl
{
	BoundingBox::BoundingBox(const Vector3f &center, float width, float height, float depth)
		:
		m_minPoint({center.x - width/2.f, center.y - height/2.f, center.z - depth/2.f}),
		m_maxPoint({center.x + width/2.f, center.y + height/2.f, center.z + depth/2.f}),
		m_size({width, height, depth}),
		m_center(center)
	{

	}

	BoundingBox::BoundingBox(const Vector3f &minPoint, const Vector3f &maxPoint)
		:
		m_minPoint(minPoint),
		m_maxPoint(maxPoint),
		m_size({maxPoint.x-minPoint.x, maxPoint.y-minPoint.y, maxPoint.z-minPoint.z}),
		m_center({maxPoint.x - m_size.x/2.f, maxPoint.y - m_size.y/2.f, maxPoint.z - m_size.z/2.f})
	{

	}


	bool BoundingBox::intersects(const BoundingBox &other) const
	{
		// Other box outside, right, above, behind
		if(other.m_minPoint.x >= m_maxPoint.x ||
			other.m_minPoint.y >= m_maxPoint.y ||
			other.m_minPoint.z >= m_maxPoint.z)
			return false;

		// Other box outside, left, below, infront
		if(other.m_maxPoint.x <= m_minPoint.x ||
			other.m_maxPoint.y <= m_minPoint.y ||
			other.m_maxPoint.z <= m_minPoint.z)
			return false;

		return true;
	}

	BoundingBox& BoundingBox::enlarge(const BoundingBox &newBox)
	{
		// Enlarge x min/max
		if(newBox.getMinPoint().x < m_minPoint.x)
			m_minPoint.x = newBox.getMinPoint().x;
		if(newBox.getMaxPoint().x > m_maxPoint.x)
			m_maxPoint.x = newBox.getMaxPoint().x;

		// Enlarge y min/max
		if(newBox.getMinPoint().y < m_minPoint.y)
			m_minPoint.y = newBox.getMinPoint().y;
		if(newBox.getMaxPoint().y > m_maxPoint.y)
			m_maxPoint.y = newBox.getMaxPoint().y;

		// Enlarge z min/max
		if(newBox.getMinPoint().z < m_minPoint.z)
			m_minPoint.z = newBox.getMinPoint().z;
		if(newBox.getMaxPoint().z > m_maxPoint.z)
			m_maxPoint.z = newBox.getMaxPoint().z;

		return *this;
	}

	void BoundingBox::setSize(const Vector3f &size)
	{
		m_size = size;
	}
	void BoundingBox::setCenter(const Vector3f &center)
	{
		m_center = center;
	}
	void BoundingBox::setPoints(const Vector3f &minPoint, const Vector3f &maxPoint)
	{
		m_minPoint = minPoint;
		m_maxPoint = maxPoint;
	}

	bool BoundingBox::operator == (const BoundingBox &rhs)
	{
		return getMinPoint() == rhs.getMinPoint() && getMaxPoint() == rhs.getMaxPoint();
	}
	bool BoundingBox::operator != (const BoundingBox &rhs)
	{
		return getMinPoint() != rhs.getMinPoint() || getMaxPoint() != rhs.getMaxPoint();
	}

	const Vector3f& BoundingBox::getCenter() const
	{
		return m_center;
	}

	const Vector3f& BoundingBox::getSize() const
	{
		return m_size;
	}
	float BoundingBox::getArea() const
	{
		return m_size.x*m_size.y*m_size.z;
	}
	float BoundingBox::getPerimeter() const
	{
		return 4*m_size.x + 4*m_size.y + 4*m_size.z;
	}

	const Vector3f& BoundingBox::getMinPoint() const
	{
		return m_minPoint;
	}
	const Vector3f& BoundingBox::getMaxPoint() const
	{
		return m_maxPoint;
	}
};