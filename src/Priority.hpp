#ifndef JL_PRIORITY_VALUES_HPP
#define JL_PRIORITY_VALUES_HPP

namespace jl
{

	typedef unsigned char PriorityType;
	class Priority
	{
	public:
		
		// Highest priority level - used only for the most crucial of things
		const static PriorityType Highest = 255;

		// High priority level - used for the more important things
		const static PriorityType High = 128;

		// Medium priority level - used for the most common of things
		const static PriorityType Medium = 64;

		// Low priority level - used for things whose importance is next to none
		const static PriorityType Low = 32;
	};
};

#endif