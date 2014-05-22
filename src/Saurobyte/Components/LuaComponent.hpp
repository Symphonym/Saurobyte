#ifndef JL_LUACOMPONENT_HPP
#define JL_LUACOMPONENT_HPP

#include <string>
#include <unordered_set>

#include <Saurobyte/Component.hpp>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
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
		int sandBox;

		// Events that the script is subscribed to
		std::unordered_set<std::string> subscribedEvents;

		LuaComponent(const std::string fileName);

		virtual std::string getName() const;
	};
};

#endif