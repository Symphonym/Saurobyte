#include <Saurobyte/AudioChunk.hpp>
#include <Saurobyte/AudioFileImpl.hpp>
#include <AL/al.h>

namespace Saurobyte
{

	AudioChunk::AudioChunk(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource)
		:
		AudioSource(std::move(audioPtr), newSource)
	{
		if(isValid())
		{
			m_file->readFileIntoBuffer(m_buffer.buffer);
			m_file->close();
			alSourceQueueBuffers(m_source, 1, &m_buffer.buffer);
		}

	}
	AudioChunk::~AudioChunk()
	{
	}

	void AudioChunk::setLooping(bool looping)
	{
		if(isValid())
			alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}
	void AudioChunk::setOffset(Time offset)
	{
		if(isValid())
		{
			ALfloat secondOffset = offset.asSeconds();
			alSourcef(m_source, AL_SEC_OFFSET, secondOffset);
		}
	}

	Time AudioChunk::getOffset() const
	{
		if(isValid())
		{
			ALfloat seconds = 0;
			alGetSourcef(m_source, AL_SEC_OFFSET, &seconds);

			return Saurobyte::seconds(seconds);
		}
		else
			return Time();
	
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