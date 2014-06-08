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

#include <Saurobyte/Time.hpp>

namespace Saurobyte
{
	Time::Time()
		:
		m_time()
	{

	}
	Time::Time(const std::chrono::nanoseconds &time)
		:
		m_time(time)
	{
		
	}

	std::int_fast64_t Time::asNanoseconds() const
	{
		return m_time.count();
	}
	std::int_fast64_t Time::asMicroseconds() const
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(m_time).count();
	}
	std::int_fast64_t Time::asMilliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_time).count();
	}
	float Time::asSeconds() const
	{
		return std::chrono::duration_cast<std::chrono::duration<float> >(m_time).count();
	}


	Time nanoseconds(std::int_fast64_t nanosecondCount)
	{
		return Time(std::chrono::nanoseconds(nanosecondCount));
	}
	Time microseconds(std::int_fast64_t microecondCount)
	{
		return Time(
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::microseconds(microecondCount)));
	}
	Time milliseconds(std::int_fast64_t millisecondCount)
	{
		return Time(
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(millisecondCount)));
	}
	Time seconds(float secondCount)
	{
		return Time(
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(secondCount)));
	}
}