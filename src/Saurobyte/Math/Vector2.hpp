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


#ifndef SAUROBUTE_VECTOR_2_HPP
#define SAUROBUTE_VECTOR_2_HPP

#include <cmath>

namespace Saurobyte
{
	/**
	 * Vector2
	 * 
	 * 2-Dimensional vector of generic component type
	 */
	template<typename TType> class Vector2
	{
	public:

		TType x, y;

		explicit Vector2(TType xCoord = 0, TType yCoord = 0)
			:
			x(xCoord),
			y(yCoord)
		{

		};


		/**
		 * Equality comparison of the components in the vectors
		 * @param  rhs The other vector
		 * @return     If the component values of rhs and this vector are equal
		 */
		bool operator==(const Vector2<TType> &rhs) const
		{
			return x == rhs.x && y == rhs.y;
		};
		/**
		 * Equality comparison of the components in the vectors
		 * @param  rhs The other vector
		 * @return     If the component values of rhs and this vector are NOT equal
		 */
		bool operator!=(const Vector2<TType> &rhs) const
		{
			return !(*this == rhs);
		};

		/**
		 * Adds the component values of rhs to respective component in this vector
		 * @param  rhs The other vector
		 * @return     A copy of the resulting vector
		 */
		Vector2<TType> operator+(const Vector2<TType> &rhs) const
		{
			return Vector2(x + rhs.x, y + rhs.y);
		};
		/**
		 * Adds the component values of rhs to respective component in this vector
		 * @param  rhs The other vector
		 * @return     A reference to this vector
		 */
		Vector2<TType>& operator +=(const Vector2<TType> &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		};

		/**
		 * Subtracts the component values of rhs from respective component in this vector
		 * @param  rhs The other vector
		 * @return     A copy of the resulting vector
		 */
		Vector2<TType> operator-(const Vector2<TType> &rhs) const
		{
			return Vector2(x - rhs.x, y - rhs.y);
		};
		/**
		 * Subtracts the component values of rhs from respective component in this vector
		 * @param  rhs The other vector
		 * @return     A reference to this vector
		 */
		Vector2<TType>& operator -=(const Vector2<TType> &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		};


		/**
		 * Scalar multiplication, multiplying the value of rhs to each component in this vector
		 * @param  rhs The scalar value
		 * @return     A reference to this vector
		 */
		Vector2<TType>& operator *=(TType rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		};

		/**
		 * Normalizes the components of this vector
		 */
		void normalize()
		{
			float len = length();
			x /= len;
			y /= len;
		};

		/**
		 * Returns a normalized copy of this vector
		 */
		Vector2<TType> normalized() const
		{
			Vector2 vec(*this);
			vec.normalize();
			return vec;
		};
		/**
		 * Calculates the distance to another vector
		 * @param  rhs The other vector
		 * @return     Distance from this vector to rhs
		 */
		float distance(const Vector2<TType> &rhs) const
		{
			return (rhs - *this).length();
		};
		/**
		 * Calculates the dot product to another vector
		 * @param  rhs The other vector
		 * @return     The dot product between the two vectors
		 */
		float dot(const Vector2<TType> &rhs) const
		{
			return x*rhs.x + y*rhs.y;
		};
		/**
		 * Calclates the length of this vector, as measured from 0,0
		 * @return The length of the vector
		 */
		float length() const
		{
			return std::sqrt((x*x) + (y*y));
		};
	};

	/**
	 * Right-hand side scalar multiplication, multiplying the value of rhs to each component in lhs 
	 * @param  lhs The vector
	 * @param  rhs The scalar value
	 * @return     A copy of the resulting vector
	 */
	template<typename TType> Vector2<TType> operator *(const Vector2<TType> &lhs, TType rhs)
	{
		return Vector2<TType>(lhs.x*rhs, lhs.y*rhs);
	};
	/**
	 * Left-hand side scalar multiplication, multiplying the value of lhs to each component in rhs 
	 * @param  lhs The vector
	 * @param  rhs The scalar value
	 * @return     A copy of the resulting vector
	 */
	template<typename TType> Vector2<TType> operator *(TType lhs, const Vector2<TType> &rhs)
	{
		return Vector2<TType>(lhs*rhs.x, lhs*rhs.y);
	};


	/**
	 * Commonly used vector types
	 */
	typedef Vector2<int> Vector2i;
	typedef Vector2<unsigned int> Vector2u;
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
};

#endif