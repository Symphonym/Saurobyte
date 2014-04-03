#include "AudioDevice.hpp"
#include "Logger.hpp"
#include "AudioSource.hpp"
#include "AudioChunk.hpp"
#include "Logger.hpp"
#include "MutexLock.hpp"


#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_assert.h>
#include <SndFile/sndfile.h>

namespace jl
{
	namespace
	{
		struct AudioData
		{
			ALuint buffer; 
			std::string fileName;
		};

		// Audio filenames and buffers mapped to string names
		std::unordered_map<std::string, AudioData> m_audioFiles;

		SDL_mutex *m_mutex = nullptr;

		std::vector<SoundHandle> m_sounds;
		std::vector<StreamHandle> m_streams;

		unsigned int m_lastCleanupTick;
		const unsigned int CleanupMSInterval = 10000;

		ALCint m_maxMonoCount = 0, m_maxStereoCount = 0;
	};


	AudioDevice::AudioDevice()
		:
		m_device(nullptr),
		m_context(nullptr)
	{
		m_mutex = SDL_CreateMutex();
		m_lastCleanupTick = SDL_GetTicks() + CleanupMSInterval;


		// Initialize OpenAL device with default settings
		m_device = alcOpenDevice(NULL);

		if(!m_device)
			JL_ERROR_LOG("Could not Initialize OpenAL device");

		m_context = alcCreateContext(m_device, NULL);

		if(!m_context || alcMakeContextCurrent(m_context))
			JL_ERROR_LOG("Could not create and set OpenAL context.");

		JL_INFO_LOG("OpenAL Vendor: %s", alGetString(AL_VENDOR));
		JL_INFO_LOG("OpenAL Version: %s", alGetString(AL_VERSION));

		alcGetIntegerv(m_device, ALC_MONO_SOURCES, 1, &m_maxMonoCount);
		alcGetIntegerv(m_device, ALC_STEREO_SOURCES, 1, &m_maxStereoCount);
		JL_INFO_LOG("OpenAL supports a maximum of %i mono sources.", m_maxMonoCount);
		JL_INFO_LOG("OpenAL supports a maximum of %i stereo sources.", m_maxStereoCount);
		//JL_INFO_LOG("OpenAL opened device: %s", alcGetString(m_device, ALC_DEFAULT_DEVICE_SPECIFIER));
	}
	AudioDevice::~AudioDevice()
	{
		SDL_DestroyMutex(m_mutex);

		for(auto itr = m_audioFiles.begin(); itr != m_audioFiles.end(); itr++)
		{
			if(alIsBuffer(itr->second.buffer))
				alDeleteBuffers(1, &itr->second.buffer);
		}

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}

	int AudioDevice::getFormatFromChannels(unsigned int channelCount)
	{
		// Find audio format based on channel count
		switch (channelCount)
		{
			case 1 : return AL_FORMAT_MONO16;
			case 2 : return AL_FORMAT_STEREO16;
			case 4 : return alGetEnumValue("AL_FORMAT_QUAD16");
			case 6 : return alGetEnumValue("AL_FORMAT_51CHN16");
			case 7 : return alGetEnumValue("AL_FORMAT_61CHN16");
			case 8 : return alGetEnumValue("AL_FORMAT_71CHN16");
			default : return 0;
		}
	}
	std::string AudioDevice::getOpenALError(ALenum errorCode)
	{
		switch(errorCode)
		{
			case AL_INVALID_ENUM:
				return "OpenAL error (AL_INVALID_ENUM): Invalid enum parameter passed to OpenAL call";
			case AL_INVALID_VALUE:
				return "OpenAL error (AL_INVALID_VALUE): Invalid value parameter passed to OpenAL call";
			case AL_INVALID_OPERATION:
				return "OpenAL error (AL_INVALID_OPERATION): Illegal OpenAL call";
			case AL_OUT_OF_MEMORY:
				return "OpenAL error (AL_OUT_OF_MEMORY): OpenAL does not have enough memory available";

			default:
				return "No error";
		}
	}

	void AudioDevice::registerAudio(const std::string &fileName, const std::string &name)
	{
		auto itr = m_audioFiles.find(name);
		if(itr == m_audioFiles.end())
		{
			AudioData data;
			data.fileName = fileName;
			data.buffer = 0;

			m_audioFiles[name] = data;
		}
	}

	StreamHandle AudioDevice::createStream(const std::string &name)
	{
		// Perform cleanup before creating new streams
		audioCleanup();

		auto itr = m_audioFiles.find(name);
		if(itr != m_audioFiles.end())
		{
			// Create new audio source
			m_streams.push_back(
				StreamHandle(new AudioStream(itr->second.fileName)));

			return m_streams.back();
		}
		else
		{
			JL_ERROR_LOG("Couldn't find any sound by the name '%s'", name.c_str());
			return StreamHandle(nullptr);
		}
	}
	StreamHandle AudioDevice::playStream(const std::string &name)
	{
		StreamHandle handle = createStream(name);

		if(handle)
			handle->play();

		return handle;
	}

	SoundHandle AudioDevice::createSound(const std::string &name)
	{
		// Perform cleanup before creating new sounds
		audioCleanup();

		auto itr = m_audioFiles.find(name);
		if(itr != m_audioFiles.end())
		{
			// Check if data hasn't been loaded for this file previously
			if(!alIsBuffer(itr->second.buffer) || itr->second.buffer == 0)
			{
				// Open file and save initial data
				SF_INFO fileInfo;
				SNDFILE *file = sf_open(itr->second.fileName.c_str(), SFM_READ, &fileInfo);

				if(file == NULL)
				{
					JL_ERROR_LOG("Could not open audio file '%s'", itr->second.fileName.c_str());
					return SoundHandle();
				}

				// Create new buffer
				alGenBuffers(1, &itr->second.buffer);

				// Read whole file
				int sampleCount = fileInfo.frames * fileInfo.channels;
				std::vector<ALshort> fileData(sampleCount);
				sf_read_short(file, &fileData[0], sampleCount);
				alBufferData(
					itr->second.buffer,
					AudioDevice::getFormatFromChannels(fileInfo.channels),
					&fileData[0],
					sampleCount*sizeof(ALushort),
					fileInfo.samplerate);

				sf_close(file);
			}

			// Create new audio source
			m_sounds.push_back(
				SoundHandle(new AudioChunk(itr->second.buffer, itr->second.fileName)));

			return m_sounds.back();
		}
		else
		{
			JL_ERROR_LOG("Couldn't find any sound by the name '%s'", name.c_str());
			return SoundHandle(nullptr);
		}
	}
	SoundHandle AudioDevice::playSound(const std::string &name)
	{
		SoundHandle handle = createSound(name);

		if(handle)
			handle->play();

		return handle;
	}

	std::size_t AudioDevice::audioCleanup()
	{
		// Perform cleanup if interval has passed
		if(SDL_TICKS_PASSED(SDL_GetTicks(), m_lastCleanupTick))
		{
			// Set tick for next cleanup
			m_lastCleanupTick = SDL_GetTicks() + CleanupMSInterval;

			std::vector<std::size_t> soundIndicesToRemove, streamIndicesToRemove;
			std::size_t soundsRemoved = 0;

			for(std::size_t i = 0; i < m_sounds.size(); i++)
			{
				SoundHandle &handle = m_sounds[i];
				if(handle.use_count() == 1 && !handle->isPlaying())
				{
					soundIndicesToRemove.push_back(i);
					++soundsRemoved;
				}
			}
			for(std::size_t i = 0; i < m_streams.size(); i++)
			{
				StreamHandle &handle = m_streams[i];
				if(handle.use_count() == 1 && !handle->isPlaying())
				{
					streamIndicesToRemove.push_back(i);
					++soundsRemoved;
				}
			}
			for(auto& index : soundIndicesToRemove)
				m_sounds.erase(m_sounds.begin() + index);
			for(auto& index : streamIndicesToRemove)
				m_streams.erase(m_streams.begin() + index);

			JL_INFO_LOG("Removed %i sounds", soundsRemoved);
			JL_INFO_LOG("Total sounds %i", getTotalSoundCount());


			return soundsRemoved;

		}
		

		return 0;
	}

	void AudioDevice::stopAllAudio()
	{
		stopStreams();
		stopSounds();

		audioCleanup();
	}
	void AudioDevice::stopStreams()
	{
		for(auto& value : m_streams)
			value->stop();
	}
	void AudioDevice::stopSounds()
	{
		for(auto& value : m_sounds)
			value->stop();
	}

	std::size_t AudioDevice::getSoundCount()
	{
		return m_sounds.size();
	}
	std::size_t AudioDevice::getStreamCount()
	{
		return m_streams.size();
	}
	std::size_t AudioDevice::getTotalSoundCount()
	{
		return m_sounds.size() + m_streams.size();
	}
};