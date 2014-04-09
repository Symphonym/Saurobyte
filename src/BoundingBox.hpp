#ifndef JL_BOUNDINGBOX_HPP
#define JL_BOUNDINGBOX_HPP

#include "Math.hpp"

namespace jl
{
	/*
		BoundingBox

		Bounding box for 3D rectangle, allowing for simple collision checking
	*/
	class BoundingBox
	{

	private:

		Vector3f m_minPoint, m_maxPoint;
		Vector3f m_size;
		Vector3f m_center;

	public:

		explicit BoundingBox(const Vector3f &minPoint, const Vector3f &maxPoint);
		explicit BoundingBox(const Vector3f &center, float width, float height, float depth);

		// Checks whether or not this bounding box collides with another, sharing the
		// same edge does not count as a collision.
		bool intersects(const BoundingBox &other) const;

		void setSize(const Vector3f &size);
		void setCenter(const Vector3f &center);
		void setPoints(const Vector3f &minPoint, const Vector3f &maxPoint);

		const Vector3f& getCenter() const;
		const Vector3f& getSize() const;

		const Vector3f& getMinPoint() const;
		const Vector3f& getMaxPoint() const;
	};
};

#endif