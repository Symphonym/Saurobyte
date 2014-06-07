#include <Saurobyte/AudioSource.hpp>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <AL/al.h>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/AudioFileImpl.hpp>

namespace Saurobyte
{
	//AudioSource::AudioSource()
	//	:
	//	m_isValidSource(false),
	//	m_thread(nullptr),
	//	m_position(0,0,0),
	//	m_source(0),
	//	m_audioStatus(AudioStatus::Stopped)
	//{

	//}
	AudioSource::AudioSource(AudioFilePtr audioFilePtr, std::uint32_t newSource)
		:
		m_isValidSource(false),
		m_thread(),
		m_position(0,0,0),
		m_source(newSource),
		m_file(std::move(audioFilePtr)),
		m_audioStatus(AudioStatus::Stopped)
	{
		// Create source
		//revalidateSource();
		m_isValidSource = m_file->isOpen();
	}

	AudioSource::~AudioSource()
	{
		stop();

		if(alIsSource(m_source) && m_isValidSource)
			alDeleteSources(1, &m_source);
	}

	std::uint32_t AudioSource::invalidate()
	{
		stop();
		m_file->close();
		m_isValidSource = false;

		std::uint32_t tempSource = m_source;
		m_source = 0;

		return tempSource;
	}

	void AudioSource::play()
	{
		if(m_isValidSource && !isPlaying())
		{
			/*auto threadFunc = [] (void *data) -> int
			{	
				AudioSource *audio = static_cast<AudioSource*>(data);
				int returnValue = audio->updateData();

				// This means that the reason for stopping the thread was that playback
				// finished.
				//if(returnValue == 0)

				return returnValue;
			};*/
			m_audioStatus = AudioSource::Playing;

			alSourcePlay(m_source);
			std::string threadName = "AudioSource(" + std::to_string(m_source) + ")";

			m_thread = std::thread(&AudioSource::updateData, this);
			//m_thread = SDL_CreateThread(threadFunc, threadName.c_str(), this);
			onPlay();
		}
	}
	void AudioSource::pause()
	{
		if(m_isValidSource && isPlaying())
		{
			m_audioStatus = AudioSource::Paused;
			alSourcePause(m_source);
			onPause();
		}
	}
	void AudioSource::stop()
	{
		if(m_isValidSource && (isPlaying() || m_audioStatus == AudioStatus::Paused))
		{
			m_audioStatus = AudioSource::Stopped;
			m_thread.join();
			//SDL_WaitThread(m_thread, NULL);
			alSourceStop(m_source);
			onStop();
		}
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
		if(m_isValidSource)
			alSourcef(m_source, AL_PITCH, pitch);
	}
	void AudioSource::setPosition(Vector3f position)
	{
		if(m_isValidSource)
		{
			m_position = position;
			alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
		}
	}
	void AudioSource::setVelocity(Vector3f velocity)
	{
		if(m_isValidSource)
			alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}
	void AudioSource::setDirection(Vector3f direction)
	{
		if(m_isValidSource)
			alSource3f(m_source, AL_DIRECTION, direction.x, direction.y, direction.z);
	}
	void AudioSource::setRelativeToListener(bool relative)
	{
		if(m_isValidSource)
			alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
	}
	void AudioSource::setLooping(bool looping)
	{
		if(m_isValidSource)
			alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}
	void AudioSource::setVolume(float volume)
	{
		if(m_isValidSource)
			alSourcef(m_source, AL_GAIN, volume);
	}

	float AudioSource::getVolume() const
	{
		if(m_isValidSource)
		{
			ALfloat volume = 0;
			alGetSourcef(m_source, AL_GAIN, &volume);
			return volume;
		}
		else
			return 0;

	}
	const Vector3f& AudioSource::getPosition() const
	{
		return m_position;
	}

	bool AudioSource::isPlaying() const
	{
		return m_audioStatus == AudioStatus::Playing;
	}
	bool AudioSource::isValid() const
	{
		return m_isValidSource;
	}





	AudioSource::BufferWrapper::BufferWrapper()
		:
		buffer(0)
	{
		alGenBuffers(1, &buffer);
	}
	AudioSource::BufferWrapper::~BufferWrapper()
	{
		alDeleteBuffers(1, &buffer);
	}
};