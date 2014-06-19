/*

	The MIT License (MIT)

	Copyright (c) 2014 by Jakob Larsson

	Permission is hereby granted, free of charge, to any person obtaining 
	a copy of this software and associated documentation files (the "Software"), 
	to deal in the Software without restriction, including without limitation the 
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
	sell copies of the Software, and to permit persons to whom the Software is 
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in 
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

#include <Saurobyte/OpenALImpl.hpp>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
{
	namespace internal
	{
		OpenALImpl::OpenALImpl()
			:
			openALPlaybackDevice(NULL),
			openALCaptureDevice(NULL),
			openALContext(NULL)
		{
			// Use default system device
			changePlaybackDevice(NULL);
			changeCaptureDevice(NULL);

			SAUROBYTE_INFO_LOG("OpenAL startup information: ");

			SAUROBYTE_INFO_LOG("\tVendor: ", alGetString(AL_VENDOR));
			SAUROBYTE_INFO_LOG("\tVersion: ", alGetString(AL_VERSION));

			ALCint maxMono = 0, maxStereo = 0;
			alcGetIntegerv(openALPlaybackDevice, ALC_MONO_SOURCES, 1, &maxMono);
			alcGetIntegerv(openALPlaybackDevice, ALC_STEREO_SOURCES, 1, &maxStereo);

			SAUROBYTE_INFO_LOG("\tSupported source (mono) count: ", maxMono);
			SAUROBYTE_INFO_LOG("\tSupported source (stereo) count: ", maxStereo);
		}
		OpenALImpl::~OpenALImpl()
		{
			alcMakeContextCurrent(NULL);
			alcDestroyContext(openALContext);
			alcCloseDevice(openALPlaybackDevice);
			alcCaptureCloseDevice(openALCaptureDevice);
		}

		void OpenALImpl::changePlaybackDevice(const ALchar *deviceName)
		{
			alcMakeContextCurrent(NULL);

			// Close devices if they're already active
			if (openALPlaybackDevice)
				alcCloseDevice(openALPlaybackDevice);
			if (openALContext)
				alcDestroyContext(openALContext);

			// Initialize OpenAL device with default settings
			openALPlaybackDevice = alcOpenDevice(deviceName);

			if(!openALPlaybackDevice)
				SAUROBYTE_FATAL_LOG("Could not initialize OpenAL playback device. ", alcGetError(openALPlaybackDevice));
			else
				SAUROBYTE_INFO_LOG("OpenAL playback device opened: ", alcGetString(openALPlaybackDevice, ALC_ALL_DEVICES_SPECIFIER));

			openALContext = alcCreateContext(openALPlaybackDevice, NULL);

			if(!openALContext || !alcMakeContextCurrent(openALContext))
				SAUROBYTE_FATAL_LOG("Could not create and set OpenAL context.");
		}
		void OpenALImpl::changeCaptureDevice(const ALchar *deviceName)
		{
			if (openALCaptureDevice)
				alcCaptureCloseDevice(openALCaptureDevice);

			const ALCuint frequency = 44110;
			openALCaptureDevice = alcCaptureOpenDevice(deviceName, 44100, AL_FORMAT_MONO16, frequency/2);
			
			if(!openALCaptureDevice)
				SAUROBYTE_ERROR_LOG("Could not initialize OpenAL capture device. ", alcGetError(openALCaptureDevice));
			else
				SAUROBYTE_INFO_LOG("OpenAL capture device opened: ", alcGetString(openALCaptureDevice, ALC_CAPTURE_DEVICE_SPECIFIER));
		}
	}
}