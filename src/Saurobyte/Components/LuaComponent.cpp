#include <Saurobyte/Components/LuaComponent.hpp>

namespace jl
{
	LuaComponent::LuaComponent(const std::string fileName)
		:
		runningStatus(LuaRunningStatuses::NotLoaded),
		luaFile(fileName)
	{}

	std::string LuaComponent::getName() const
	{
		return "LuaComponent";
	}
};