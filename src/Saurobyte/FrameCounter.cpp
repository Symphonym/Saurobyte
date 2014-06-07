#include <Saurobyte/FrameCounter.hpp>
#include <thread>

namespace Saurobyte
{

	FrameCounter::FrameCounter() :
		m_fps(0),
		m_deltaTime(0)
	{

	}

	void FrameCounter::update()
	{
		FrameClock::time_point curTick = FrameClock::now();

		// This is the time point where we "should" be according to the FPS we want
		FrameClock::time_point calculatedCurTick = m_lastTick + m_targetTickDuration;

		// If actual time is behind where we should be, sleep until we get there
		if(curTick < calculatedCurTick)
		{
			std::this_thread::sleep_for(FrameClock::duration(calculatedCurTick - curTick));
			curTick = FrameClock::now(); // Update curTick post-sleep
		}
		// TODO use sleep_until instead

		// Calculate fps and deltaTime converting from nanoseconds to seconds
		FrameClock::duration tickDuration = curTick - m_lastTick;
		m_fps = 1000000000/std::chrono::duration_cast<std::chrono::nanoseconds>(tickDuration).count();
		m_deltaTime = 
			static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(tickDuration).count())/1000000000.f;

		m_lastTick = curTick;
	}

	void FrameCounter::limitFps(unsigned int fps)
	{
		std::chrono::nanoseconds nanoSec(1000000000/fps);
		m_targetTickDuration = FrameClock::duration(nanoSec);
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