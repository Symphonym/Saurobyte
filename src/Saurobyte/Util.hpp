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


#ifndef SAUROBYTE_UTIL_HPP
#define SAUROBYTE_UTIL_HPP

#include <string>
#include <sstream>
#include <vector>

namespace Saurobyte
{


	inline void appendToStream(std::ostream &stream) {};
	template<typename TType, typename... TArgs> void appendToStream(std::ostream &stream, TType &first, const TArgs&... args)
	{
		stream << first;
		appendToStream(stream, args...);
	};

	/**
	 * Converts all arguments into a string using a string stream
	 * @param  args Arguments to convert into the string in the order they are provided
	 * @return      The string with all the arguments concatenated
	 */
	template<typename... TArgs> std::string toStr(const TArgs&... args)
	{
		std::ostringstream ss;
		appendToStream(ss, args...);
		return ss.str();
	}

	void sleep(unsigned int sleepInMs);

	std::vector<std::string> splitStr(const std::string &strToSplit, char delimiter);
};

#endif