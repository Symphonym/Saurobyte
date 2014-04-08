#include "AudioStream.hpp"
#include "AudioDevice.hpp"
#include "Logger.hpp"
#include <AL/al.h>
#include <vector>

namespace jl
{


	AudioStream::AudioStream(const std::string &filePath)
		:
		m_file(nullptr),
		m_fileInfo(),
		m_fileName(filePath),
		m_loop(false),
		m_duration(0)
	{
		alGenBuffers(m_buffers.size(), &m_buffers[0]);

		// Open file and save initial data
		m_file = sf_open(filePath.c_str(), SFM_READ, &m_fileInfo);

		if(m_file == NULL)
			JL_WARNING_LOG("Could not open audio file for streaming '%s'", filePath.c_str());

		m_fileName = filePath;
		m_duration = static_cast<float>(m_fileInfo.frames)/static_cast<float>(m_fileInfo.samplerate);

		if(m_file != NULL)
			fillBuffers();
	}
	AudioStream::~AudioStream()
	{
		alDeleteBuffers(m_buffers.size(), &m_buffers[0]);

		if(m_file != nullptr)
		{
			sf_close(m_file);
			m_file = nullptr;
		}
	}

	void AudioStream::setLooping(bool looping)
	{
		m_loop = looping;
	}
	void AudioStream::setOffset(float secondOffset)
	{
		//TODO alSourcef(m_source, AL_SEC_OFFSET, secondOffset);
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
			int sampleSecondCount = m_fileInfo.channels*m_fileInfo.samplerate;
			std::vector<ALshort> fileData(sampleSecondCount);
			int readCount = sf_read_short(m_file, &fileData[0], sampleSecondCount);

			// If looping is enabled and we reached end of file, just move to the
			// beginning of the file and start reading there.
			if(readCount < sampleSecondCount && m_loop)
			{
				//m_playingOffset = 0;
				sf_seek(m_file, 0, SEEK_SET);
				readCount = sf_read_short(m_file, &fileData[0], sampleSecondCount);
			}

			// Queue more data into audio stream
			if(readCount > 0)
			{
				alBufferData(
					buffer,
					AudioDevice::getFormatFromChannels(m_fileInfo.channels),
					&fileData[0],
					sizeof(ALushort)*sampleSecondCount,
					m_fileInfo.samplerate);
				alSourceQueueBuffers(m_source, 1, &buffer);
			}
		

			--processedBuffers;
		}
	}

	void AudioStream::fillBuffers()
	{
		// Load second chunks of data
		std::size_t sampleSecondCount = m_fileInfo.channels*m_fileInfo.samplerate;

		std::vector<ALshort> fileData(sampleSecondCount);

		// Read 1s chunks of data into the buffers
		for(int i = 0; i < StreamBufferCount; i++)
		{
			sf_read_short(m_file, &fileData[0], sampleSecondCount);
			alBufferData(
				m_buffers[i],
				AudioDevice::getFormatFromChannels(m_fileInfo.channels),
				&fileData[0],
				sampleSecondCount*sizeof(ALushort),
				m_fileInfo.samplerate);

		}
		alSourceQueueBuffers(m_source, StreamBufferCount, &m_buffers[0]);
	}
};