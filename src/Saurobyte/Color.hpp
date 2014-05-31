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


#ifndef SAUROBYTE_COLOR_HPP
#define SAUROBYTE_COLOR_HPP

#include <Saurobyte/ApiDefines.hpp>

namespace Saurobyte
{
	class SAUROBYTE_API Color
	{
	public:

		typedef unsigned char ColorComponent;

		/**
		 * Creates a color with the specified RGBA values
		 * @param  r Red value, 0-255
		 * @param  g Green value, 0-255
		 * @param  b Blue value, 0-255
		 * @param  a Alpha value, 0-255
		 */
		explicit Color(ColorComponent r, ColorComponent g, ColorComponent b, ColorComponent a = 255);

		ColorComponent r, g, b, a;

		Color operator + (const Color &rhs);
		Color& operator += (const Color &rhs);

		Color operator - (const Color &rhs);
		Color& operator -= (const Color &rhs);

		/**
		 * Predefined colors
		 */
		const static Color Red;
		const static Color Green;
		const static Color Blue;

	};
};

#endif