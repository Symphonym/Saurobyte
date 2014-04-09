#include "BoundingBox.hpp"

namespace jl
{
	BoundingBox::BoundingBox(const Vector3f &minPoint, const Vector3f &maxPoint)
		:
		m_minPoint(minPoint),
		m_maxPoint(maxPoint),
		m_size({maxPoint.x-minPoint.x, maxPoint.y-minPoint.y, maxPoint.z-minPoint.z}),
		m_center({maxPoint.x - m_size.x/2.f, maxPoint.y - m_size.y/2.f, maxPoint.z - m_size.z/2.f})
	{

	}
	BoundingBox::BoundingBox(const Vector3f &center, float width, float height, float depth)
		:
		m_minPoint({center.x - width, center.y - height, center.z - depth}),
		m_maxPoint({center.x + width, center.y + height, center.z + depth}),
		m_size({width, height, depth}),
		m_center(center)
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

	const Vector3f& BoundingBox::getCenter() const
	{
		return m_center;
	}

	const Vector3f& BoundingBox::getSize() const
	{
		return m_size;
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