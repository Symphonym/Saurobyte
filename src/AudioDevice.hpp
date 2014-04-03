#ifndef JL_AUDIODEVICE_HPP
#define JL_AUDIODEVICE_HPP

#include <string>
#include <AL/alc.h>
#include <AL/al.h>
#include <memory>
#include "AudioChunk.hpp"
#include "AudioStream.hpp"

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

		static std::size_t audioCleanup();

	public:

		static int getFormatFromChannels(unsigned int channelCount);
		static std::string getOpenALError(ALenum error);

		static void registerAudio(const std::string &fileName, const std::string &name);

		static StreamHandle createStream(const std::string &name);
		static StreamHandle playStream(const std::string &name);

		static SoundHandle createSound(const std::string &name);
		static SoundHandle playSound(const std::string &name);


		// Stops all running sounds
		static void stopAllAudio();

		// Stops only streams/sounds respectively
		static void stopStreams();
		static void stopSounds();

		// Get size of sound pools
		static std::size_t getSoundCount();
		static std::size_t getStreamCount();
		static std::size_t getTotalSoundCount();
	};
};

#endif