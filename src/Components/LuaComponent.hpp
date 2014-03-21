#ifndef JL_LUACOMPONENT_HPP
#define JL_LUACOMPONENT_HPP

#include "Component.hpp"
#include <string>

namespace jl
{
	class LuaComponent : public Component<LuaComponent>
	{
	public:

		enum LuaRunningStatuses
		{
			NotLoaded,
			Running,
			Errors
		};
		LuaRunningStatuses runningStatus;
		std::string luaFile;

		LuaComponent(const std::string fileName)
			:
			runningStatus(LuaRunningStatuses::NotLoaded),
			luaFile(fileName)
		{}

	};
};

#endif