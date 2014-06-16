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
			openALDevice(NULL),
			openALContext(NULL)
		{

			// Initialize OpenAL device with default settings
			openALDevice = alcOpenDevice(NULL);

			if(!openALDevice)
				SAUROBYTE_FATAL_LOG("Could not Initialize OpenAL device");

			openALContext = alcCreateContext(openALDevice, NULL);

			if(!openALContext || !alcMakeContextCurrent(openALContext))
				SAUROBYTE_FATAL_LOG("Could not create and set OpenAL context.");

			SAUROBYTE_INFO_LOG("OpenAL Vendor: ", alGetString(AL_VENDOR));
			SAUROBYTE_INFO_LOG("OpenAL Version: ", alGetString(AL_VERSION));

			ALCint maxMono = 0, maxStereo = 0;
			alcGetIntegerv(openALDevice, ALC_MONO_SOURCES, 1, &maxMono);
			alcGetIntegerv(openALDevice, ALC_STEREO_SOURCES, 1, &maxStereo);
			SAUROBYTE_INFO_LOG("OpenAL supports a maximum of: ", maxMono, " mono source(s) and ", maxStereo, " stereo source(s)");
			//SAUROBYTE_INFO_LOG("OpenAL opened device: %s", alcGetString(openALDevice, ALC_DEFAULT_DEVICE_SPECIFIER));
		}
		OpenALImpl::~OpenALImpl()
		{
			SAUROBYTE_INFO_LOG("DESTROYING AL");
			alcMakeContextCurrent(NULL);
			alcDestroyContext(openALContext);
			alcCloseDevice(openALDevice);
		}
	}
}