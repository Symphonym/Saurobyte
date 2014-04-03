#include "AudioChunk.hpp"
#include <AL/al.h>
#include <SDL2/SDL_timer.h>
#include "Logger.hpp"

namespace jl
{
	AudioChunk::AudioChunk(unsigned int buffer, const std::string &fileName)
		:
		m_buffer(buffer),
		m_fileName(fileName)
	{
		// Queue our buffer
		alSourceQueueBuffers(m_source, 1, &buffer);

	}

	void AudioChunk::setBuffer(unsigned int buffer, const std::string &fileName)
	{
		alSourcei(m_source, AL_BUFFER, 0);

		m_fileName = fileName;
		m_buffer = buffer;
		alSourceQueueBuffers(m_source, 1, &m_buffer);
	}

	std::string AudioChunk::getFileName() const
	{
		return m_fileName;
	}
};