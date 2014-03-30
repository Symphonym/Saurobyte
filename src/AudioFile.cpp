#include "AudioFile.hpp"
#include "AudioDevice.hpp"
#include <vector>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>

namespace jl
{

	AudioFile::AudioFile()
		:
		m_source(0),
		m_buffers({0,0,0}),
		m_readingType(ReadingTypes::Load),
		m_audioStatus(AudioStatuses::Stopped),
		m_fileInfo(),
		m_file(nullptr),
		m_fileName(""),
		m_hasLoadedFile(false),
		m_updateThread(nullptr)
	{
		alGenSources(1, &m_source);
	}

	AudioFile::~AudioFile()
	{
		for(std::size_t i = 0; i < m_buffers.size(); i++)
		{
			if(alIsBuffer(m_buffers[i]))
				alDeleteBuffers(1, &m_buffers[i]);
		}

		alDeleteSources(1, &m_source);

		if(m_hasLoadedFile && m_readingType != ReadingTypes::Load)
		{
			sf_close(m_file);
			m_file = nullptr;
		}

		// Force updating thread to finish
		m_audioStatus = AudioStatuses::Stopped;
		SDL_WaitThread(m_updateThread, NULL);
	}

	void AudioFile::readFile(const std::string &fileName, ReadingTypes readingType)
	{
		// Close any previously loaded file (only applies to 
		// streamed files since loaded files close after reading the file)
		if(m_file && m_readingType != ReadingTypes::Load)
		{
			sf_close(m_file);
			m_file = nullptr;
		}
		else
			m_hasLoadedFile = true;

		// Open file and save initial data
		m_file = sf_open(fileName.c_str(), SFM_READ, &m_fileInfo);
		m_fileName = fileName;
		m_readingType = readingType;

		// Cleanup, rewind the currently active buffer and clear any
		// buffers attached to the source.
		alSourceRewind(m_source);
		alSourcei(m_source, AL_BUFFER, 0);

		if(readingType == ReadingTypes::Load)
		{
			alGenBuffers(1, &m_buffers[0]);
			std::size_t sampleCount = m_fileInfo.frames*m_fileInfo.channels;

			std::vector<ALshort> fileData(sampleCount);

			// Read whole file
			sf_read_short(m_file, &fileData[0], sampleCount);
			alBufferData(
				m_buffers[0],
				AudioDevice::getFormatFromChannels(m_fileInfo.channels),
				&fileData[0],
				sampleCount*sizeof(ALushort),
				m_fileInfo.samplerate);

			alSourceQueueBuffers(m_source, 1, &m_buffers[0]);

			// Close file after we've read it, since we're not streaming
			sf_close(m_file);
			m_file = nullptr;

		}
		else if(readingType == ReadingTypes::Stream)
		{
			alGenBuffers(StreamBufferCount, &m_buffers[0]);
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

				alSourceQueueBuffers(m_source, StreamBufferCount, &m_buffers[0]);
			}

		}
	}

	void AudioFile::play()
	{
		if(m_hasLoadedFile && m_audioStatus != AudioStatuses::Playing)
		{
			auto updateFunc = [] (void *data) -> int
			{
				AudioFile* file = static_cast<AudioFile*>(data);
				file->updateData();
				return 0;
			};

			m_audioStatus = AudioStatuses::Playing;
			alSourcePlay(m_source);

			m_updateThread = SDL_CreateThread(updateFunc, std::string("SoundFileThread(" + m_fileName + ")").c_str(), this);
		}
	}
	void AudioFile::pause()
	{
		if(m_hasLoadedFile && m_audioStatus == AudioStatuses::Playing)
		{
			alSourcePause(m_source);
			m_audioStatus = AudioStatuses::Paused;
		}
	}
	void AudioFile::stop()
	{
		if(m_hasLoadedFile && m_audioStatus == AudioStatuses::Playing)
		{
			alSourceStop(m_source);

			// If we're streaming this will only reset the currently used buffer
			// i.e resetting only a second of playtime
			alSourceRewind(m_source);

			// Move reading head of stream to start of file if source is stopped
			if(m_readingType == ReadingTypes::Stream)
				sf_seek(m_file, 0, SEEK_SET);

			m_audioStatus = AudioStatuses::Stopped;

			SDL_WaitThread(m_updateThread, NULL);
		}
	}
	void AudioFile::setPlayingOffset(float offset)
	{
		alSourcef(m_source, AL_SEC_OFFSET, offset);
	}
	void AudioFile::setLooping(bool looping)
	{
		alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
	}

	float AudioFile::getPlayingOffset() const
	{
		ALfloat offset = 0;
		alGetSourcef(m_source, AL_SEC_OFFSET, &offset);
		return offset;//(m_fileInfo.frames)/m_fileInfo.samplerate;
	}
	std::size_t AudioFile::getDuration() const
	{
		return m_fileInfo.frames/m_fileInfo.samplerate;
	}
	bool AudioFile::isPlaying() const
	{
		return m_audioStatus == AudioStatuses::Playing;
	}


	void AudioFile::updateData()
	{
		
		ALint processedBuffers = 0, state = 0;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processedBuffers);

		while(m_audioStatus == AudioStatuses::Playing)
		{
			// Stream data, if it's a stream
			if(m_readingType == ReadingTypes::Stream && processedBuffers > 0)
			{
				// Pop used buffers from the queue and recycle them
				ALuint buffer;
				alSourceUnqueueBuffers(m_source, 1, &buffer);

				// Read a 1s chunk of data
				int sampleSecondCount = m_fileInfo.channels*m_fileInfo.samplerate;
				std::vector<ALshort> fileData(sampleSecondCount);
				int readCount = sf_read_short(m_file, &fileData[0], sampleSecondCount);

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


			// If playback stopped without us stopping it, just play it again
			if(state != AL_PLAYING && m_audioStatus == AudioStatuses::Playing)
				alSourcePlay(m_source);

			// We don't need to update every frame, so spare some CPU
			SDL_Delay(10);


			alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processedBuffers);
			alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		}
	}
};