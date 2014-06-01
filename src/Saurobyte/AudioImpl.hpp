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
		struct BufferWrapper
		{
			ALuint buffer;

			explicit BufferWrapper(ALuint newBuffer)
				:
				buffer(newBuffer)
			{

			}
			~BufferWrapper()
			{
				alDeleteBuffers(1, &buffer);
			}
		};

		template<unsigned int bufferCount> class SAUROBYTE_API AudioImpl
		{
		public:

			ALuint source;
			std::array<std::shared_ptr<BufferWrapper>, bufferCount> buffers;


			AudioImpl(ALuint newSource)
				:
				source(newSource),
				m_file(NULL)
			{
				
			}
			~AudioImpl()
			{
				close();
			}

			void open(const std::string &filePath)
			{
				// Open file and save initial data
				m_file = sf_open(filePath.c_str(), SFM_READ, &m_fileInfo);

				if(m_file == NULL)
					SAUROBYTE_WARNING_LOG("Could not open audio file '", filePath, "'");
			}
			void close()
			{
				if(m_file != NULL)
				{
					sf_close(m_file);
					m_file = NULL;
				}
			}

			/**
			 * Reads from the currently opened file one second worth of audio data
			 * @param bufferIndex The buffer in which to store the audio data
			 */
			/**
			 * Reads from the currently opened file one second worth of audio data
			 * @param allowLooping If set to true, if less than one second of data was read it moves to the beginning of the file
			 * @param bufferIndex  The buffer in which to store the audio data
			 */
			void readSecondIntoBuffer(bool allowLooping, int bufferIndex = 0)
			{
				// Read one second chunk of data
				int sampleSecondCount = m_fileInfo.channels*m_fileInfo.samplerate;
				std::vector<ALshort> fileData(sampleSecondCount);
				int readCount = sf_read_short(m_file, &fileData[0], sampleSecondCount);

				// Not enough data was read
				if(readCount < sampleSecondCount && allowLooping)
				{
					// Move to start of file
					sf_seek(m_file, 0, SEEK_SET);

					// Keep reading data until we have all the data we need
					while(readCount < sampleSecondCount)
					{
						int leftToRead = sampleSecondCount - readCount;
						int newReadCount = sf_read_short(m_file, &fileData[0], leftToRead);

						readCount += newReadCount;

						// Keep re-reading the file until we have all the data we want
						if(newReadCount < leftToRead)
							sf_seek(m_file, 0, SEEK_SET);
					}
				}

				alBufferData(
					buffer[bufferIndex],
					AudioDevice::getFormatFromChannels(m_fileInfo.channels),
					&fileData[0],
					sampleSecondCount*sizeof(ALushort),
					m_fileInfo.samplerate);
			}
			/**
			 * Reads from the currently opened file all audio data
			 * @param bufferIndex The buffer in which to store the audio data
			 */
			void readFileIntoBuffer(int bufferIndex = 0)
			{
				// Read whole file
				int sampleCount = m_fileInfo.frames * m_fileInfo.channels;
				std::vector<ALshort> fileData(sampleCount);
				sf_read_short(file, &fileData[0], sampleCount);
				
				alBufferData(
					buffers[bufferIndex],
					AudioDevice::getFormatFromChannels(m_fileInfo.channels),
					&fileData[0],
					sampleCount*sizeof(ALushort),
					m_fileInfo.samplerate);
			}

			void save(const std::string &filePath)
			{
				// TODO
			}

		private:

			SNDFILE *m_file;
			SF_INFO m_fileInfo;
		};
	}

	
}