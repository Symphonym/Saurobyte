/*

	The MIT License (MIT)

	Copyright (c) 2014 by Jakob Larsson

	Permission is hereby granted, free of charge, to any person obtaining 
	a copy of this software and associated documentation files (the "Software"), 
	to deal in the Software without restriction, including without limitation the 
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
	sell copies of the Software, and to permit persons to whom the Software is 
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in 
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */


#include <Saurobyte/AudioSource.hpp>
#include <Saurobyte/AudioFileImpl.hpp>
#include <AL/al.h>

namespace Saurobyte
{

	AudioSource::AudioSource(AudioFilePtr audioFilePtr, std::uint32_t newSource)
		:
		m_isValidSource(false),
		m_position(0,0,0),
		m_source(newSource),
		m_file(std::move(audioFilePtr))
	{
		m_isValidSource = m_file->isOpen();

		// Multi-channel sample count
		unsigned int sampleCount = m_file->getFileInfo().frames*m_file->getFileInfo().channels;

		m_duration = Saurobyte::seconds(
			(static_cast<float>(sampleCount) / 
			static_cast<float>(m_file->getFileInfo().samplerate)) /
			static_cast<float>(m_file->getFileInfo().channels));
	}

	AudioSource::~AudioSource()
	{
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

	const Time& AudioSource::getDuration() const
	{
		return m_duration;
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
		return getStatus() == AudioStatus::Playing;
	}
	AudioSource::AudioStatus AudioSource::getStatus() const
	{
		if(!isValid())
			return AudioStatus::Stopped;

		ALint state = AL_INITIAL;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		switch(state)
		{
			case AL_PLAYING: return AudioStatus::Playing;
			case AL_PAUSED: return AudioStatus::Paused;
			default: return AudioStatus::Stopped;
		}
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