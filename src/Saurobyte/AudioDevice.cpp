#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/AudioSource.hpp>
#include <Saurobyte/AudioChunk.hpp>
#include <Saurobyte/AudioListener.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/OpenALImpl.hpp>
#include <Saurobyte/AudioFileImpl.hpp>

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
		// Map string ID's to audio filepaths
		std::unordered_map<std::string, std::string> m_audioFiles;

		typedef std::pair<PriorityType, AudioHandle> SoundData;

		// Active audio sources
		std::vector<SoundData> m_sounds;

		// Sounds can be played through channels which allows their volume
		// to be set collectively. TODO
		//std::unordered_map<std::string, float> m_audioChannels;
		
	};


	AudioDevice::AudioDevice()
		:
		m_openAL(new internal::OpenALImpl())
	{

	}
	AudioDevice::~AudioDevice()
	{

	}

	void AudioDevice::registerAudio(const std::string &fileName, const std::string &name)
	{
		auto itr = m_audioFiles.find(name);
		if(itr == m_audioFiles.end())
			m_audioFiles[name] = fileName;
	}

	AudioHandle AudioDevice::createStream(const std::string &name, PriorityType priority)
	{
		auto itr = m_audioFiles.find(name);
		std::string fileName = name;

		if(itr != m_audioFiles.end())
			fileName = itr->second;

		AudioSource::AudioFilePtr filePtr(new internal::AudioFileImpl());
		filePtr->open(fileName);

		std::uint32_t newSource = 0;

		// If the sound wasen't given a file, it will not be given a source since it
		// would be wasteful. (The AudioSource is invalid)
		if(filePtr->isOpen())
			newSource = grabAudioSource();

		AudioHandle handle = AudioHandle(new AudioStream(std::move(filePtr), newSource));

		// Valid sounds are stored in the audio device and returned
		if(handle->isValid())
		{
			m_sounds.push_back(std::make_pair(priority, handle));
			return m_sounds.back().second;
		}

		// Invalid sounds are simply returned since they don't contain a valid
		// source which could be reused.
		else
			return handle;
	}
	AudioHandle AudioDevice::playStream(const std::string &name, PriorityType priority)
	{
		AudioHandle handle = createStream(name, priority);
		handle->play();

		return handle;
	}

	AudioHandle AudioDevice::createSound(const std::string &name, PriorityType priority)
	{
		auto itr = m_audioFiles.find(name);
		std::string fileName = name;

		if(itr != m_audioFiles.end())
			fileName = itr->second;

		AudioSource::AudioFilePtr filePtr(new internal::AudioFileImpl());
		filePtr->open(fileName);

		std::uint32_t newSource = 0;

		// If the sound wasen't given a file, it will not be given a source since it
		// would be wasteful. (The AudioSource is invalid)
		if(filePtr->isOpen())
			newSource = grabAudioSource();

		AudioHandle handle = AudioHandle(new AudioChunk(std::move(filePtr), newSource));

		// Valid sounds are stored in the audio device and returned
		if(handle->isValid())
		{
			m_sounds.push_back(std::make_pair(priority, handle));
			return m_sounds.back().second;
		}

		// Invalid sounds are simply returned since they don't contain a valid
		// source which could be reused.
		else
			return handle;
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

	void AudioDevice::wipeSource(std::uint32_t source)
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

	std::uint32_t AudioDevice::grabAudioSource()
	{
		std::uint32_t newSource = 0;

		// Try to find a completely unused source
		for(std::size_t i = 0; i < m_sounds.size(); i++)
		{
			AudioHandle &handle = m_sounds[i].second;
			if(!handle->isPlaying() && handle.use_count() == 1)
			{
				newSource = handle->invalidate();
				wipeSource(newSource);
				m_sounds.erase(m_sounds.begin() + i);
				return newSource;
			}
		}

		// No source could be recycled, try alternative means
		alGetError(); // Clear errors
		alGenSources(1, &newSource);

		ALenum sourceError = alGetError();

		// If a new source couldn't be generated, force source reusage
		if (sourceError != AL_NO_ERROR)
			newSource = leastImportantSource();

		// Wipe old data off source before returning it
		wipeSource(newSource);
		return newSource;
	}


	std::uint32_t AudioDevice::leastImportantSource()
	{

		// Little utility function for erasing the element at the front of the
		// sound list and grab its source.
		auto eraseAndReturn = [] () -> std::uint32_t
		{
			AudioHandle &handle = m_sounds[0].second;
			std::uint32_t newSource = handle->invalidate();

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
				++lowestPriorityCount;
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
				++lowestVolumeCount;
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
				++furthestDistanceCount;
		}

		// Only one sound had furthest distance away, return it
		if(furthestDistanceCount == 0)
			return eraseAndReturn();

		// Sort tied distances by their playback time left
		std::sort(m_sounds.begin(), m_sounds.begin() + furthestDistanceCount,
			[](const SoundData &lhs, const SoundData &rhs) -> bool
			{
				return (lhs.second->getDuration().asSeconds()-lhs.second->getOffset().asSeconds()) <
					(rhs.second->getDuration().asSeconds()-rhs.second->getOffset().asSeconds());
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