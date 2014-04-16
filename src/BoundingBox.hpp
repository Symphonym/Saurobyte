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

		// Default constructor is 1x1x1 cube centered at origo
		explicit BoundingBox(const Vector3f &center = {0,0,0}, float width = 1, float height = 1, float depth = 1);
		explicit BoundingBox(const Vector3f &minPoint, const Vector3f &maxPoint);

		// Checks whether or not this bounding box collides with another, sharing the
		// same edge does not count as a collision.
		bool intersects(const BoundingBox &other) const;

		// Enlarges this bounding box so that 'newBox' fits within it
		BoundingBox& enlarge(const BoundingBox &newBox);

		void setSize(const Vector3f &size);
		void setCenter(const Vector3f &center);
		void setPoints(const Vector3f &minPoint, const Vector3f &maxPoint);

		bool operator == (const BoundingBox &rhs);
		bool operator != (const BoundingBox &rhs);


		const Vector3f& getCenter() const;
		const Vector3f& getSize() const;
		float getArea() const;
		float getPerimeter() const;

		const Vector3f& getMinPoint() const;
		const Vector3f& getMaxPoint() const;
	};
};

#endif