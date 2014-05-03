#ifndef JL_FRAMECOUNTER_HPP
#define JL_FRAMECOUNTER_HPP

#include <SDL2/SDL.h>

namespace jl
{

	class FrameCounter
	{
	private:

		// Frame rate calculating data
		unsigned int m_frames;
		unsigned int m_fps;
		unsigned int m_lastSecondTick;

		// Frame limiting data
		unsigned int m_lastTick;
		unsigned int m_targetTickDuration; // How long each tick should last

		// Frame duration data
		Uint64 m_highPefLastTick; // High performance counter value
		float m_deltaTime; // Time between frames, in seconds

	public:

		FrameCounter();

		void update();
		void limitFps(unsigned int fps);

		unsigned int getFps() const;
		float getDelta() const;

	};
};

#endif