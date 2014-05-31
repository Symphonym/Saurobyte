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


#include <Saurobyte/Color.hpp>

namespace Saurobyte
{
	const Color Color::Red = Color(255, 0, 0);
	const Color Color::Green = Color(0, 255, 0);
	const Color Color::Blue = Color(0, 0, 255);

	Color::Color(ColorComponent newR, ColorComponent newG, ColorComponent newB, ColorComponent newA)
		:
		r(newR),
		g(newG),
		b(newB),
		a(newA)
	{

	}

	Color Color::operator + (const Color &rhs)
	{
		Color color(*this);
		color.r += rhs.r;
		color.g += rhs.g;
		color.b += rhs.b;
		color.a += rhs.a;

		return color;
	}
	Color& Color::operator += (const Color &rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	Color Color::operator - (const Color &rhs)
	{
		Color color(*this);
		color.r -= rhs.r;
		color.g -= rhs.g;
		color.b -= rhs.b;
		color.a -= rhs.a;

		return color;
	}
	Color& Color::operator -= (const Color &rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}
};