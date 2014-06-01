#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/AudioSource.hpp>
#include <Saurobyte/AudioChunk.hpp>
#include <Saurobyte/AudioListener.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/OpenALImpl.hpp>

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SndFile/sndfile.h>

namespace Saurobyte
{
	namespace
	{
		struct AudioData
		{
			// Use a handle so we can keep track of how many audio sources
			// that uses this buffer.
			AudioBufferHandle buffer; 
			std::string fileName;
		};

		// Map string ID's to audio filepaths
		std::unordered_map<std::string, std::string> m_audioFiles;

		typedef std::pair<PriorityType, AudioHandle> SoundData;

		// Active audio sources
		std::vector<SoundData> m_sounds;
		std::vector<StreamHandle> m_streams;

		// Sounds can be played through channels which allows their volume
		// to be set collectively. TODO
		//std::unordered_map<std::string, float> m_audioChannels;
		unsigned int m_sourceCount = 0; // TODO use these
		unsigned int m_sourceLimit = 0;

		unsigned int m_lastCleanupTick;
		const unsigned int CleanupMSInterval = 10000;

		
	};


	AudioDevice::AudioDevice()
		:
		m_openAL(nullptr)
	{
		m_lastCleanupTick = SDL_GetTicks() + CleanupMSInterval;

		m_openAL = std::unique_ptr<internal::OpenALImpl>(new internal::OpenALImpl());

	}
	AudioDevice::~AudioDevice()
	{
		//for(auto itr = m_audioFiles.begin(); itr != m_audioFiles.end(); itr++)
		//{
		//	if(alIsBuffer(*itr->second.buffer))
		//		alDeleteBuffers(1, itr->second.buffer.get());
		//}
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

	void AudioDevice::registerAudio(const std::string &fileName, const std::string &name)
	{
		auto itr = m_audioFiles.find(name);
		if(itr == m_audioFiles.end())
		{
			//AudioData data;
			//data.fileName = fileName;
			//data.buffer = AudioBufferHandle(new unsigned int(0));

			m_audioFiles[name] = fileName;//data;
		}
	}

	AudioHandle AudioDevice::createStream(const std::string &name, PriorityType priority)
	{
		auto itr = m_audioFiles.find(name);
		if(itr != m_audioFiles.end())
		{
			unsigned int newSource = grabAudioSource();

			// Create new stream
			m_sounds.push_back(
				std::make_pair(
					priority,
					AudioHandle(new AudioStream(newSource, itr->second))));

			//freeForStream(m_streams.back().get());
			return m_sounds.back().second;
		}
		else
		{
			SAUROBYTE_WARNING_LOG("Couldn't find any stream by the name '", name, "'");
			return AudioHandle(new AudioStream());
		}
	}
	AudioHandle AudioDevice::playStream(const std::string &name, PriorityType priority)
	{
		AudioHandle handle = createStream(name, priority);
		handle->play();

		return handle;
	}

	AudioHandle AudioDevice::createSound(const std::string &name, PriorityType priority)
	{
		/*auto itr = m_audioFiles.find(name);
		if(itr != m_audioFiles.end())
		{
			// Check if data hasn't been loaded for this file previously
			if(!alIsBuffer(*itr->second.buffer) || *itr->second.buffer == 0)
			{
				// Open file and save initial data
				SF_INFO fileInfo;
				SNDFILE *file = sf_open(itr->second.fileName.c_str(), SFM_READ, &fileInfo);

				if(file == NULL)
				{
					SAUROBYTE_WARNING_LOG("Could not open audio file '", itr->second.fileName, "'");
					return AudioHandle(new AudioChunk());
				}

				// Create new buffer
				alGenBuffers(1, itr->second.buffer.get());

				// Read whole file
				int sampleCount = fileInfo.frames * fileInfo.channels;
				std::vector<ALshort> fileData(sampleCount);
				sf_read_short(file, &fileData[0], sampleCount);
				alBufferData(
					*itr->second.buffer,
					AudioDevice::getFormatFromChannels(fileInfo.channels),
					&fileData[0],
					sampleCount*sizeof(ALushort),
					fileInfo.samplerate);

				sf_close(file);
			}

			unsigned int newSource = grabAudioSource();

			m_sounds.push_back(
				std::make_pair(
					priority,
					AudioHandle(new AudioChunk(newSource, itr->second.buffer, itr->second.fileName))
				));

			return m_sounds.back().second;

		}
		else
		{
			SAUROBYTE_WARNING_LOG("Couldn't find any sound by the name '", name, "'");
			return AudioHandle(new AudioChunk());
		}*/
	}
	AudioHandle AudioDevice::playSound(const std::string &name, PriorityType priority)
	{
		AudioHandle handle = createSound(name, priority);
		handle->play();

		return handle;
	}


	void AudioDevice::bufferCleanup()
	{
		/*std::size_t buffersRemoved = 0;

		// Clean buffers
		for(auto itr = m_audioFiles.begin(); itr != m_audioFiles.end(); itr++)
		{
			if(itr->second.buffer.use_count() == 1)
			{
				if(alIsBuffer(*itr->second.buffer))
					alDeleteBuffers(1, itr->second.buffer.get());

				*itr->second.buffer = 0;
				++buffersRemoved;
			}
		}

		if(buffersRemoved > 0)
			SAUROBYTE_DEBUG_LOG("Removed ", buffersRemoved, " audio buffers");*/
	}

	void AudioDevice::wipeSource(unsigned int source)
	{
		// Rewind the active buffer, might just rewind the source, I don't know :I
		alSourceRewind(source);
		alSourcei(source, AL_BUFFER, 0); // Clear buffers

		// Reset data to defaults
		alSourcei(source, AL_LOOPING, AL_FALSE);
		alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
		alSourcef(source, AL_GAIN, 1);
		alSourcef(source, AL_PITCH, 1.f);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSource3f(source, AL_DIRECTION, 0, 0, 0);

	}

	unsigned int AudioDevice::grabAudioSource()
	{
		unsigned int newSource = -1;

		// Try to find an unused source
		for(std::size_t i = 0; i < m_sounds.size(); i++)
		{
			AudioHandle &handle = m_sounds[i].second;
			if(!handle->isPlaying())
			{
				newSource = handle->m_source;

				handle->m_source = 0;
				handle->m_isValidSource = false;
				m_sounds.erase(m_sounds.begin() + i);
				break;
			}
		}

		// No source could be recycled, try alternative means
		if(newSource == -1)
		{
			alGetError(); // Clear errors
			alGenSources(1, &newSource);

			ALenum sourceError = alGetError();

			// If a new source couldn't be generated, force source reusage
			if (sourceError != AL_NO_ERROR)
				newSource = leastImportantSource();
		}

		// Wipe old data off source before returning it
		wipeSource(newSource);
		return newSource;
	}


	unsigned int AudioDevice::leastImportantSource()
	{

		// Little utility function for erasing the element at the front of the
		// sound list and grab its source.
		auto eraseAndReturn = [] () -> unsigned int
		{
			AudioHandle &handle = m_sounds[0].second;
			handle->stop();
			unsigned int newSource = handle->m_source;

			handle->m_source = 0;
			handle->m_isValidSource = false;
			m_sounds.erase(m_sounds.begin());

			return newSource;
		};

		// Sort sounds by priority
		std::sort(m_sounds.begin(), m_sounds.end(),
			[](const SoundData &lhs, const SoundData &rhs) -> bool
			{
				return lhs.first < rhs.first;
			});

		// Check how many elements are tied at the same lowest priority
		PriorityType lowestPriority = m_sounds[0].first;
		unsigned int lowestPriorityCount = 0;
		for(std::size_t i = 0; i < m_sounds.size(); i++)
		{
			if(m_sounds[i].first != lowestPriority)
				break;
			else
				lowestPriorityCount += 1;
		}

		// Only one sound of the lowest available priority was found, return it
		if(lowestPriorityCount == 0)
			return eraseAndReturn();

		// Sort tied priorities by volume
		std::sort(m_sounds.begin(), m_sounds.begin() + lowestPriorityCount,
			[](const SoundData &lhs, const SoundData &rhs) -> bool
			{
				return lhs.second->getVolume() < rhs.second->getVolume();
			});

		// Check how many elements are tied on the same volume level
		float lowestVolume = m_sounds[0].second->getVolume();
		unsigned int lowestVolumeCount = 0;
		for(std::size_t i = 0; i < m_sounds.size(); i++)
		{
			if(m_sounds[i].second->getVolume() != lowestVolume)
				break;
			else
				lowestVolumeCount += 1;
		}

		// Only one sound of the lowest volume was found, return it
		if(lowestVolumeCount == 0)
			return eraseAndReturn();

		// Sort tied volumes by their distance to the listener
		std::sort(m_sounds.begin(), m_sounds.begin() + lowestVolumeCount,
			[](const SoundData &lhs, const SoundData &rhs) -> bool
			{
				return lhs.second->getPosition().distance(AudioListener::getPosition()) >
					 rhs.second->getPosition().distance(AudioListener::getPosition());
			});

		// Check how many elements are tied on the same playback left
		float furthestDistance = m_sounds[0].second->getPosition().distance(AudioListener::getPosition());
		unsigned int furthestDistanceCount = 0;
		for(std::size_t i = 0; i < m_sounds.size(); i++)
		{
			float curDistance = m_sounds[i].second->getPosition().distance(AudioListener::getPosition());
			if(curDistance != furthestDistance)
				break;
			else
				furthestDistanceCount += 1;
		}

		// Only one sound had furthest distance away, return it
		if(furthestDistanceCount == 0)
			return eraseAndReturn();

		// Sort tied distances by their playback time left
		std::sort(m_sounds.begin(), m_sounds.begin() + furthestDistanceCount,
			[](const SoundData &lhs, const SoundData &rhs) -> bool
			{
				return (lhs.second->getDuration()-lhs.second->getOffset()) <
					(rhs.second->getDuration()-rhs.second->getOffset());
			});

		// The sound at the bottom of the sorted list will be used no matter what,
		// enough effort has been given to make sure it's fitting for overwriting
		return eraseAndReturn();

	}

	void AudioDevice::stopAllAudio()
	{
		for(auto& value : m_sounds)
			value.second->stop();
		SAUROBYTE_DEBUG_LOG("Stopped all audio playback");
	}



	std::size_t AudioDevice::getTotalSoundCount()
	{
		return m_sounds.size();
	}
};