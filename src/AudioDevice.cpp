#include "AudioDevice.hpp"
#include "Logger.hpp"

namespace jl
{
	AudioDevice::AudioDevice()
		:
		m_device(nullptr),
		m_context(nullptr)
	{
		// Initialize OpenAL device with default settings
		m_device = alcOpenDevice(NULL);

		if(!m_device)
			JL_ERROR_LOG("Could not Initialize OpenAL device");

		m_context = alcCreateContext(m_device, NULL);

		if(!m_context || alcMakeContextCurrent(m_context))
			JL_ERROR_LOG("Could not create and set OpenAL context.");

		JL_INFO_LOG("OpenAL Vendor: %s", alGetString(AL_VENDOR));
		JL_INFO_LOG("OpenAL Version: %s", alGetString(AL_VERSION));
		//JL_INFO_LOG("OpenAL opened device: %s", alcGetString(m_device, ALC_DEFAULT_DEVICE_SPECIFIER));
	}
	AudioDevice::~AudioDevice()
	{
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
};