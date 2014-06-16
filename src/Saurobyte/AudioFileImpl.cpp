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


#include <Saurobyte/AudioFileImpl.hpp>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
{
	namespace internal
	{

		AudioFileImpl::AudioFileImpl()
			:
			m_file(NULL)
		{

		}
		AudioFileImpl::~AudioFileImpl()
		{
			close();
		}

		bool AudioFileImpl::open(const std::string &filePath)
		{
			// Open file and save initial data
			m_file = sf_open(filePath.c_str(), SFM_READ, &m_fileInfo);

			if(m_file == NULL)
			{
				SAUROBYTE_WARNING_LOG("Could not open audio file '", filePath, "'");
				return false;
			}
			else
				return true;
		}
		void AudioFileImpl::close()
		{
			if(m_file != NULL)
			{
				sf_close(m_file);
				m_file = NULL;
			}
		}
		bool AudioFileImpl::isOpen() const
		{
			return m_file != NULL;
		}

		void AudioFileImpl::setReadingOffset(float seconds)
		{
			sf_seek(m_file, m_fileInfo.samplerate*seconds, SEEK_SET);
		}
		bool AudioFileImpl::readSecondIntoBuffer(ALuint buffer, bool allowLooping)
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
					int newReadCount = sf_read_short(m_file, fileData.data() + readCount, leftToRead);

					readCount += newReadCount;

					// Keep re-reading the file until we have all the data we want
					if(newReadCount < leftToRead)
						sf_seek(m_file, 0, SEEK_SET);
				}
			}

			if(readCount > 0)
			{
				alBufferData(
					buffer,
					AudioFileImpl::getFormatFromChannels(m_fileInfo.channels),
					&fileData[0],
					sampleSecondCount*sizeof(ALushort),
					m_fileInfo.samplerate);
				return true;
			}
			else
				return false;
		}
		void AudioFileImpl::readFileIntoBuffer(ALuint buffer)
		{
			// Read whole file
			int sampleCount = m_fileInfo.frames * m_fileInfo.channels;
			std::vector<ALshort> fileData(sampleCount);
			sf_read_short(m_file, &fileData[0], sampleCount);
			
			alBufferData(
				buffer,
				AudioFileImpl::getFormatFromChannels(m_fileInfo.channels),
				&fileData[0],
				sampleCount*sizeof(ALushort),
				m_fileInfo.samplerate);
		}

		void AudioFileImpl::save(const std::string &filePath)
		{
			// TODO
		}

		const SF_INFO& AudioFileImpl::getFileInfo() const
		{
			return m_fileInfo;
		}

		ALenum AudioFileImpl::getFormatFromChannels(unsigned int channelCount)
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
	}
}