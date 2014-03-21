#include "FrameCounter.hpp"
#include <SDL2/SDL.h>

namespace jl
{

	FrameCounter::FrameCounter() :
		m_frames(0),
		m_fps(0),
		m_lastSecondTick(SDL_GetTicks()),
		m_lastTick(SDL_GetTicks()),
		m_targetTickDuration(0),
		m_highPefLastTick(SDL_GetPerformanceCounter()),
		m_deltaTime(0)
	{

	}

	void FrameCounter::update()
	{
		unsigned int curTick = SDL_GetTicks();
		Uint64 highPefCurTick = SDL_GetPerformanceCounter();
		++m_frames;

		m_deltaTime = (float)(highPefCurTick - m_highPefLastTick) * (1.0f/SDL_GetPerformanceFrequency());
		
		// Limit deltaTime to avoid buggy behaviour at FPS spikes
		if(m_deltaTime > 0.25)
			m_deltaTime = 0.25;

		// Frame rate limiting
		if(m_targetTickDuration > 0)
		{
			unsigned int targetTicks = m_lastTick + m_targetTickDuration;

			// If the current elapsed time isn't where we're supposed to be, sleep until we get there
			if(curTick < targetTicks)
			{
				SDL_Delay(targetTicks - curTick);

				// Update curTicks since we used SDL_Delay
				curTick = SDL_GetTicks();
			} 

		}

		// Update FPS every second
		if(curTick - m_lastSecondTick >= 1000)
		{
			m_fps = m_frames;
			m_lastSecondTick = curTick;
			m_frames = 0;
		}

		m_highPefLastTick = highPefCurTick;
		m_lastTick = curTick;
	}

	void FrameCounter::limitFps(unsigned int fps)
	{
		m_targetTickDuration = 1000.f/(float)fps;
		//m_tickLeftOvers =  ((1000.f/(float)fps) - m_targetTickDuration) * fps;
		//SDL_Log("LeftOvers(total): %f", m_tickLeftOvers);
		//SDL_Log("LeftOvers(per tick): %f", m_tickLeftOvers/fps);
	}

	unsigned int FrameCounter::getFps() const
	{
		return m_fps;
	}
	float FrameCounter::getDelta() const
	{
		return m_deltaTime;
	}

}