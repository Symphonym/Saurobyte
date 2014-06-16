#include <Saurobyte/FrameCounter.hpp>
#include <thread>

namespace Saurobyte
{

	FrameCounter::FrameCounter()
		:
		m_fps(0),
		m_deltaTime(0)
	{

	}

	void FrameCounter::update()
	{

		// This is the time point where we "should" be according to the FPS we want
		FrameClock::time_point calculatedCurTick = m_lastTick + m_targetTickDuration;

		std::this_thread::sleep_until(calculatedCurTick); // Sleep until we get there
		FrameClock::time_point curTick = FrameClock::now();

		// Calculate fps and deltaTime converting from nanoseconds to seconds
		FrameClock::duration tickDuration = curTick - m_lastTick;
		m_deltaTime = std::chrono::duration_cast<std::chrono::duration<float> >(tickDuration).count();
		m_fps = 1.0f / m_deltaTime;

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