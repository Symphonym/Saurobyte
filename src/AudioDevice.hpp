#ifndef JL_AUDIODEVICE_HPP
#define JL_AUDIODEVICE_HPP

#include <string>
#include <AL/alc.h>
#include <AL/al.h>

namespace jl
{
	class AudioDevice
	{
	private:

		// Only the Game can create an AudioDevice
		friend class Game;

		ALCdevice *m_device;
		ALCcontext *m_context;

		AudioDevice();
		~AudioDevice();

	public:

		static int getFormatFromChannels(unsigned int channelCount);
		static std::string getOpenALError(ALenum error); 
	};
};

#endif