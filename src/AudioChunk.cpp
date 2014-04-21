#include "AudioChunk.hpp"
#include <AL/al.h>
#include <SDL2/SDL_timer.h>
#include "Logger.hpp"

namespace jl
{
	AudioChunk::AudioChunk(AudioBufferHandle buffer, const std::string &fileName)
		:
		m_buffer(buffer),
		m_fileName(fileName)
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
	}

	void AudioChunk::setLooping(bool looping)
	{
		alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}
	void AudioChunk::setOffset(float secondOffset)
	{
		alSourcef(m_source, AL_SEC_OFFSET, secondOffset);
	}

	std::string AudioChunk::getFileName() const
	{
		return m_fileName;
	}
};