#include <Saurobyte/Components/LuaComponent.hpp>
#include <Lua/lua.hpp>

namespace Saurobyte
{
	LuaComponent::LuaComponent(const std::string fileName)
		:
		runningStatus(LuaRunningStatuses::NotLoaded),
		luaFile(fileName),
		sandBox(LUA_NOREF)
	{}

	std::string LuaComponent::getName() const
	{
		return "LuaComponent";
	}
};