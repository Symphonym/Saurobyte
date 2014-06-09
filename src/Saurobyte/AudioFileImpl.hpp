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


#ifndef SAUROBYTE_AUDIO_IMPL_HPP
#define SAUROBYTE_AUDIO_IMPL_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <SndFile/sndfile.h>
#include <AL/al.h>
#include <array>
#include <memory>
#include <string>

namespace Saurobyte
{
	namespace internal
	{
		class SAUROBYTE_API AudioFileImpl
		{
		public:

			AudioFileImpl();
			~AudioFileImpl();

			bool open(const std::string &filePath);
			void close();

			bool isOpen() const;

			/**
			 * Sets the offset of the reading head the specified amount of seconds into the audio file
			 * @param seconds Offset in seconds into the audio file, starting from the beginning
			 */
			void setReadingOffset(float seconds);

			/**
			 * Reads from the currently opened file one second worth of audio data
			 * @param buffer       The buffer in which to store the audio data
			 * @param allowLooping If set to true, if less than one second of data was read it moves to the beginning of the file
			 */
			void readSecondIntoBuffer(ALuint buffer, bool allowLooping = false);
			/**
			 * Reads from the currently opened file all audio data
			 * @param buffer The buffer in which to store the audio data
			 */
			void readFileIntoBuffer(ALuint buffer);

			void save(const std::string &filePath);

			/**
			 * Calculates the length (in seconds) of the currently loaded audio file
			 * @return Audio file length (in seconds)
			 */
			const SF_INFO& getFileInfo() const;

		private:

			static ALenum getFormatFromChannels(unsigned int channelCount);

			SNDFILE *m_file;
			SF_INFO m_fileInfo;
		};
	}

	
}

#endif