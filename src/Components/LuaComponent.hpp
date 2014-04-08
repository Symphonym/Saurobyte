#ifndef JL_LUACOMPONENT_HPP
#define JL_LUACOMPONENT_HPP

#include <string>
#include <unordered_set>

#include "Component.hpp"
#include "Logger.hpp"

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

		// Events that the script is subscribed to
		std::unordered_set<std::string> subscribedEvents;

		LuaComponent(const std::string fileName);

		virtual std::string getName() const;
	};
};

#endif