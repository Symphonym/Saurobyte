#ifndef JL_NONCOPYABLE_HPP
#define JL_NONCOPYABLE_HPP

namespace Saurobyte
{
	/*
		NonCopyable

		Deriving from NonCopyab;e disallows copying of the derived class.

	*/
	class NonCopyable
	{
	protected:

		NonCopyable() {};
		~NonCopyable() {};

		NonCopyable(const NonCopyable &other) = delete;
		NonCopyable& operator= (const NonCopyable &other) = delete;
	};
};

#endif