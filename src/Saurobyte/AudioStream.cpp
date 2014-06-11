#include <Saurobyte/AudioStream.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/Logger.hpp>
#include <AL/al.h>
#include <Saurobyte/AudioFileImpl.hpp>
#include <vector>

namespace Saurobyte
{

	AudioStream::AudioStream(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource)
		:
		AudioSource(std::move(audioPtr), newSource),
		m_loop(false)
	{
		if(isValid())
		{
			for(std::size_t i = 0; i < m_buffers.size(); i++)
			{
				m_file->readSecondIntoBuffer(m_buffers[i].buffer);
				alSourceQueueBuffers(m_source, 1, &m_buffers[i].buffer);
			}
		}
		
	}
	AudioStream::~AudioStream()
	{
	
	}


	void AudioStream::setLooping(bool looping)
	{
		m_loop = looping;
	}
	void AudioStream::setOffset(Time offset)
	{
		if(isValid())
		{
			stop();
			alSourceRewind(m_source);
			alSourcei(m_source, AL_BUFFER, 0); // Clear buffers

			m_file->setReadingOffset(offset.asSeconds());
			for(std::size_t i = 0; i < m_buffers.size(); i++)
			{
				alSourceUnqueueBuffers(m_source, 1, &m_buffers[i].buffer);
				m_file->readSecondIntoBuffer(m_buffers[i].buffer, m_loop);
				alSourceQueueBuffers(m_source, 1, &m_buffers[i].buffer);
			}
		}
		
		//TODO alSourcef(m_source, AL_SEC_OFFSET, secondOffset);
	}

	Time AudioStream::getOffset() const
	{
		//return 0;
	}
	bool AudioStream::isLooping() const
	{
		return m_loop;
	}


	void AudioStream::onUpdate()
	{
		ALint processedBuffers = 0;
		alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processedBuffers);

		// Process finished buffers
		while(processedBuffers > 0)
		{
			// Grab playing offset, usually around 1 (second)
			ALfloat secOffset = 0;
			alGetSourcef(m_source, AL_SEC_OFFSET, &secOffset);
			//m_playingOffset += secOffset;

			// Pop used buffers from the queue and recycle them
			ALuint buffer;
			alSourceUnqueueBuffers(m_source, 1, &buffer);

			// Read a 1s chunk of data
			m_file->readSecondIntoBuffer(buffer, m_loop);
			alSourceQueueBuffers(m_source, 1, &buffer);

			--processedBuffers;
		}
	}
};