#include "AudioSource.hpp"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <AL/al.h>
#include "Logger.hpp"
#include "AudioDevice.hpp"

namespace jl
{
	AudioSource::AudioSource()
		:
		m_isValidSource(false),
		m_audioCleanupTick(0),
		m_thread(nullptr),
		m_source(0),
		m_audioStatus(AudioStatus::Stopped)
	{
		// Create source
		revalidateSource();
	}

	AudioSource::~AudioSource()
	{
		stop();

		if(alIsSource(m_source) && m_isValidSource)
		{
		JL_INFO_LOG("DELETERINO SOUNDERINO");

			alDeleteSources(1, &m_source);
			m_isValidSource = false; // Just because
		}
	}

	void AudioSource::play()
	{
		if(m_isValidSource && !isPlaying())
		{
			m_audioCleanupTick = SDL_GetTicks();
			auto threadFunc = [] (void *data) -> int
			{	
				AudioSource *audio = static_cast<AudioSource*>(data);
				int returnValue = audio->updateData();

				// This means that the reason for stopping the thread was that playback
				// finished.
				//if(returnValue == 0)

				return returnValue;
			};
			m_audioStatus = AudioSource::Playing;

			alSourcePlay(m_source);
			std::string threadName = "AudioSource(" + std::to_string(m_source) + ")";
			m_thread = SDL_CreateThread(threadFunc, threadName.c_str(), this);
			onPlay();
		}
		else if(!m_isValidSource)
			JL_WARNING_LOG("You are attemping to play an invalid sound!");
	}
	void AudioSource::pause()
	{
		if(m_isValidSource && isPlaying())
		{
			m_audioCleanupTick = SDL_GetTicks();
			m_audioStatus = AudioSource::Paused;
			alSourcePause(m_source);
			onPause();
		}
		else if(!m_isValidSource)
			JL_WARNING_LOG("You are attemping to pause an invalid sound!");
	}
	void AudioSource::stop()
	{
		if(m_isValidSource && (isPlaying() || m_audioStatus == AudioStatus::Paused))
		{
			m_audioCleanupTick = SDL_GetTicks();
			m_audioStatus = AudioSource::Stopped;
			SDL_WaitThread(m_thread, NULL);
			alSourceStop(m_source);
			onStop();
		}
		else if(!m_isValidSource)
			JL_WARNING_LOG("You are attemping to stop an invalid sound!");
	}
	int AudioSource::updateData()
	{
		ALint state = -1;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		while(isPlaying())
		{
			// Spare some CPU
			SDL_Delay(10);

			alGetSourcei(m_source, AL_SOURCE_STATE, &state);

			// Playback has finished
			if(state == AL_STOPPED && m_audioStatus != AudioStatus::Stopped)
			{
				m_audioStatus = AudioStatus::Stopped;
				return 0;
			}

			// If the playback paused without reason, resume the playback
			if(state == AL_PAUSED && m_audioStatus != AudioStatus::Paused)
				alSourcePlay(m_source);

			onUpdate();
		}

		return 1;
	}

	void AudioSource::setPitch(float pitch)
	{
		alSourcef(m_source, AL_PITCH, pitch);
	}
	void AudioSource::setPosition(Vector3f position)
	{
		alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
	}
	void AudioSource::setVelocity(Vector3f velocity)
	{
		alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}
	void AudioSource::setDirection(Vector3f direction)
	{
		alSource3f(m_source, AL_DIRECTION, direction.x, direction.y, direction.z);
	}
	void AudioSource::setRelativeToListener(bool relative)
	{
		alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
	}
	void AudioSource::setLooping(bool looping)
	{
		alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}
	void AudioSource::setVolume(float volume)
	{
		alSourcei(m_source, AL_GAIN, volume);
	}

	bool AudioSource::revalidateSource()
	{
		// Generate a new audio source
		if(!m_isValidSource)
		{
			alGetError(); // Clear errors
			alGenSources(1, &m_source);
			ALenum sourceError = alGetError();

			if (sourceError != AL_NO_ERROR)
			{
				JL_WARNING_LOG(
					"OpenAL failed to generate a new source. This audio is very unlikely to play. (%s)",
					AudioDevice::getOpenALError(sourceError).c_str());

				return false;
			}
			else if(sourceError == AL_NO_ERROR)
			{
				m_isValidSource = true;

				// Rewind the active buffer
				alSourceRewind(m_source);
				alSourcei(m_source, AL_BUFFER, 0); // Clear buffers

				return true;
			}
		}

		return false;
	}

	bool AudioSource::isPlaying() const
	{
		return m_audioStatus == AudioStatus::Playing;
	}
	bool AudioSource::isValid() const
	{
		return m_isValidSource;
	}
};