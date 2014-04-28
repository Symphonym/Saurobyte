#ifndef JL_AUDIODEVICE_HPP
#define JL_AUDIODEVICE_HPP

#include <string>
#include <AL/alc.h>
#include <AL/al.h>
#include <memory>
#include "AudioChunk.hpp"
#include "AudioStream.hpp"
#include "Priority.hpp"

namespace jl
{
	// Handles to audio data
	typedef std::shared_ptr<AudioSource> AudioHandle;
	typedef std::shared_ptr<AudioChunk> SoundHandle;
	typedef std::shared_ptr<AudioStream> StreamHandle;


	class AudioDevice
	{
	private:

		// Only the Game can create an AudioDevice
		friend class Game;

		ALCdevice *m_device;
		ALCcontext *m_context;

		AudioDevice();
		~AudioDevice();

		// Deleletes unused audio memory, TODO
		static void bufferCleanup();

		// Grabs an OpenAL source handle to use
		static unsigned int grabAudioSource();

		// Finds the least important source that is currently active
		static unsigned int leastImportantSource();

		// Wipe the specified source's data to default
		static void wipeSource(unsigned int source);

	public:

		// Add channel support
		// store audio etc values by mapped channel names
		// set these values to audios put in that channel TODO

		static int getFormatFromChannels(unsigned int channelCount);

		static void registerAudio(const std::string &fileName, const std::string &name);

		static AudioHandle createStream(const std::string &name, PriorityType priority = Priority::Medium);
		static AudioHandle playStream(const std::string &name, PriorityType priority = Priority::Medium);

		static AudioHandle createSound(const std::string &name, PriorityType priority = Priority::Medium);
		static AudioHandle playSound(const std::string &name, PriorityType priority = Priority::Medium);


		// Stops all running sounds
		static void stopAllAudio();

		// Get size of sound pools
		static std::size_t getTotalSoundCount();
	};
};

#endif