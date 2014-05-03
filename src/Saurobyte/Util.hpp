#ifndef SAUROBYTE_UTIL_HPP
#define SAUROBYTE_UTIL_HPP

#include <string>
#include <sstream>

namespace Saurobyte
{


	inline void appendToStream(std::ostream &stream) {};
	template<typename TType, typename... TArgs> void appendToStream(std::ostream &stream, TType &first, const TArgs&... args)
	{
		stream << first;
		appendToStream(stream, args...);
	};

	template<typename... TArgs> std::string toStr(const TArgs&... args)
	{
		std::ostringstream ss;
		appendToStream(ss, args...);
		return ss.str();
	}
};

#endif