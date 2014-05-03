#ifndef SAUROBUTE_VECTOR_2_HPP
#define SAUROBUTE_VECTOR_2_HPP

#include <cmath>

namespace Saurobyte
{
	/**
	 * Vector3
	 * 
	 * 3-Dimensional vector of generic component type
	 */
	template<typename TType> class Vector3
	{
	public:

		TType x, y, z;

		explicit Vector3(TType xCoord, TType yCoord, TType zCoord)
			:
			x(xCoord),
			y(yCoord),
			z(zCoord)
		{

		};

		bool operator==(const Vector3<TType> &rhs) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}
		bool operator!=(const Vector3<TType> &rhs) const
		{
			return !(*this == rhs);
		}

		Vector3<TType> operator+(const Vector3<TType> &rhs)
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		Vector3<TType>& operator +=(const Vector3<TType> &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3<TType> operator-(const Vector3<TType> &rhs)
		{
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		Vector3<TType>& operator -=(const Vector3<TType> &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		/**
		 * Normalizes the components of this vector
		 */
		void normalize()
		{
			float len = length();
			x /= len;
			y /= len;
			z /= len;
		}

		/**
		 * Returns a normalized copy of this vector
		 */
		Vector3<TType> normalized() const
		{
			Vector3 vec(*this);
			vec.normalize();
			return vec;
		}
		/**
		 * Calculates the distance to another vector
		 * @param  rhs The other vector
		 * @return     Distance from this vector to rhs
		 */
		float distance(const Vector3<TType> &rhs) const
		{
			return (rhs - *this).length();
		};
		/**
		 * Calculates the dot product to another vector
		 * @param  rhs The other vector
		 * @return     The dot product between the two vectors
		 */
		float dot(const Vector3<TType> &rhs) const
		{
			return x*rhs.x + y*rhs.y + z*rhs.z;
		};
		/**
		 * Calclates the length of this vector, as measured from 0,0
		 * @return The length of the vector
		 */
		float length() const
		{
			return std::sqrt((x*x) + (y*y) + (z*z));
		}
	};

	/**
	 * Commonly used vector types
	 */
	typedef Vector3<int> Vector3i;
	typedef Vector3<unsigned int> Vector3u;
	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
};

#endif