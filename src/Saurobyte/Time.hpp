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


#ifndef SAUROBYTE_TIME_HPP
#define SAUROBYTE_TIME_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <cstdint>
#include <chrono>

namespace Saurobyte
{
	class SAUROBYTE_API Time
	{
	public:

		/**
		 * Constructs an empty Time object, with the time set to 0 seconds
		 */
		Time();

		/**
		 * Returns the time as Nanoseconds (10^-9)
		 * @return Time in nanoseconds
		 */
		std::int_fast64_t asNanoseconds() const;
		/**
		 * Returns the time as Microseconds (10^-6)
		 * @return Time in microseconds
		 */
		std::int_fast64_t asMicroseconds() const;
		/**
		 * Returns the time as Milliseconds
		 * @return Time in milliseconds
		 */
		std::int_fast64_t asMilliseconds() const;
		/**
		 * Returns the time as Seconds
		 * @return Time in seconds
		 */
		float asSeconds() const;

	private:

		friend SAUROBYTE_API Time nanoseconds(std::int_fast64_t nanosecondCount);
		friend SAUROBYTE_API Time microseconds(std::int_fast64_t microecondCount);
		friend SAUROBYTE_API Time milliseconds(std::int_fast64_t millisecondCount);
		friend SAUROBYTE_API Time seconds(float secondCount);

		/**
		 * Creates a Time object from a value in nanoseconds
		 * @param  time Time in nanoseconds
		 */
		explicit Time(const std::chrono::nanoseconds &time);
		std::chrono::nanoseconds m_time;
	};

	/**
	 * Creates a Time object from nanoseconds
	 * @param  nanosecondCount Time in nanoseconds
	 * @return                 Time object created from the passed nanoseconds
	 */
	SAUROBYTE_API Time nanoseconds(std::int_fast64_t nanosecondCount);
	/**
	 * Creates a Time object from microseconds
	 * @param  microecondCount Time in microseconds
	 * @return                 Time object created from the passed microseconds
	 */
	SAUROBYTE_API Time microseconds(std::int_fast64_t microecondCount);
	/**
	 * Creates a Time object from milliseconds
	 * @param  millisecondCount Time in milliseconds
	 * @return                  Time object created from the passed milliseconds
	 */
	SAUROBYTE_API Time milliseconds(std::int_fast64_t millisecondCount);
	/**
	 * Creates a Time ojbect from seconds
	 * @param  secondCount Time in seconds
	 * @return             Time object created from the passed seconds
	 */
	SAUROBYTE_API Time seconds(float secondCount);
}

#endif