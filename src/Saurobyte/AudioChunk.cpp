#include <Saurobyte/AudioChunk.hpp>
#include <AL/al.h>
#include <SDL2/SDL_timer.h>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
{

	AudioChunk::AudioChunk()
		:
		m_buffer(0),
		m_fileName(""),
		m_duration(0)
	{

	}
	AudioChunk::AudioChunk(unsigned int source, AudioBufferHandle buffer, const std::string &fileName)
		:
		AudioSource(source),
		m_buffer(buffer),
		m_fileName(fileName),
		m_duration(0)
	{
		// Queue our buffer
		alSourceQueueBuffers(m_source, 1, buffer.get());

	}

	void AudioChunk::setBuffer(AudioBufferHandle buffer, const std::string &filePath)
	{
		stop();
		alSourcei(m_source, AL_BUFFER, 0);

		m_buffer = buffer;
		m_fileName = filePath;

		// Queue our buffer
		alSourceQueueBuffers(m_source, 1, buffer.get());

		// Calculate length from buffer data
		ALint byteSize = 0;
		ALint channelCount = 0;
		ALint bitPerSample = 0;
		ALint frequency = 0;

		alGetBufferi(*buffer, AL_SIZE, &byteSize);
		alGetBufferi(*buffer, AL_CHANNELS, &channelCount);
		alGetBufferi(*buffer, AL_BITS, &bitPerSample);
		alGetBufferi(*buffer, AL_FREQUENCY, &frequency);

		float sampleLength = (byteSize * 8) / (channelCount * bitPerSample);
		m_duration = sampleLength / static_cast<float>(frequency);
	}

	void AudioChunk::setLooping(bool looping)
	{
		alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}
	void AudioChunk::setOffset(float secondOffset)
	{
		alSourcef(m_source, AL_SEC_OFFSET, secondOffset);
	}

	float AudioChunk::getOffset() const
	{
		ALfloat seconds = 0;
		alGetSourcef(m_source, AL_SEC_OFFSET, &seconds);

		return seconds;
	}
	float AudioChunk::getDuration() const
	{
		return m_duration;
	}
	bool AudioChunk::isLooping() const
	{
		ALint looping = 0;
		alGetSourcei(m_source, AL_LOOPING, &looping);

		return looping;
	}

	std::string AudioChunk::getFileName() const
	{
		return m_fileName;
	}
};