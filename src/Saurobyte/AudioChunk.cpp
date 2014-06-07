#include <Saurobyte/AudioChunk.hpp>
#include <AL/al.h>
#include <SDL2/SDL_timer.h>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/AudioFileImpl.hpp>

namespace Saurobyte
{

	AudioChunk::AudioChunk(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource)
		:
		AudioSource(std::move(audioPtr), newSource),
		m_duration(0)
	{
		//m_buffer->open(filePath);
		//m_buffer->readFileIntoBuffer(m_buffer->buffers[0].buffer);
		//m_buffer->close();
		//m_file->readFileIntoBuffer()
		//m_buffer = AudioSource::ALBufferPtr(new internal::ALBufferWrapper());
		//m_file.readFileIntoBuffer(m_buffer->buffer);
		//m_file.close();
		if(isValid())
		{
			m_file->readFileIntoBuffer(m_buffer.buffer);
			m_file->close();
			alSourceQueueBuffers(m_source, 1, &m_buffer.buffer);

			m_duration = 
				m_file->getFileInfo().frames / m_file->getFileInfo().samplerate;

			SAUROBYTE_INFO_LOG("DURATION ", m_duration, "s");
			//float sampleLength = (byteSize * 8) / (channelCount * bitPerSample);
		//m_duration = sampleLength / static_cast<float>(frequency);*/
		}


		// Queue our buffer
		//alSourceQueueBuffers(m_source, 1, &m_buffer->buffers[0].buffer);

	}
	AudioChunk::~AudioChunk()
	{
	}

	void AudioChunk::setLooping(bool looping)
	{
		if(isValid())
			alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}
	void AudioChunk::setOffset(float secondOffset)
	{
		if(isValid())
			alSourcef(m_source, AL_SEC_OFFSET, secondOffset);
	}

	float AudioChunk::getOffset() const
	{
		if(isValid())
		{
			ALfloat seconds = 0;
			alGetSourcef(m_source, AL_SEC_OFFSET, &seconds);

			return seconds;
		}
		else
			return -1;
	
	}
	float AudioChunk::getDuration() const
	{
		return m_duration;
	}
	bool AudioChunk::isLooping() const
	{
		if(isValid())
		{
			ALint looping = 0;
			alGetSourcei(m_source, AL_LOOPING, &looping);

			return looping;
		}
		else
			return false;
	}
};