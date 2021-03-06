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


#include <Saurobyte/AudioStream.hpp>
#include <Saurobyte/AudioFileImpl.hpp>
#include <Saurobyte/Util.hpp>
#include <al.h>

namespace Saurobyte
{

	AudioStream::AudioStream(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource)
		:
		AudioSource(std::move(audioPtr), newSource),
		m_loop(false),
		m_requestStop(false)
	{
		// TODO streaming has undefined behaviour since occasionally it will get stuck
		// in streaming and play the same segment over and over even though the file is
		// continously read and eventually the end of the file is reached, as it should, which
		// is weird seeing how that audio never reaches the stream buffers.
	}
	AudioStream::~AudioStream()
	{
		// Make sure to stop the streaming thread
		m_requestStop = true;

		if(m_thread.joinable())
			m_thread.join();
	}

	void AudioStream::play()
	{
		if(isValid() && !isPlaying())
		{
			m_requestStop = false;

			// Only start streaming thread if it isn't already active
			if(getStatus() == AudioSource::Stopped && !m_thread.joinable())
			{
				prepareStreaming();
				m_thread = std::thread(&AudioStream::processStream, this);
			}
			
			alSourcePlay(m_source);
		}
		
	}
	void AudioStream::pause()
	{
		if(isValid() && isPlaying())
			alSourcePause(m_source);
	}
	void AudioStream::stop()
	{
		if(isValid() && isPlaying())
		{
			// Reset offset and stop the OpenAL source
			m_playingOffset = Time();
			m_requestStop = true;

			if(m_thread.joinable())
				m_thread.join();

			alSourceStop(m_source); // TODO this was previously above the thread joining, check if better
			m_file->setReadingOffset(0);
		}
	}

	void AudioStream::prepareStreaming()
	{
		ALint bufferCount = 0;
		alGetSourcei(m_source, AL_BUFFERS_QUEUED, &bufferCount);

		// Clear buffer queue
		ALuint tempBuffer = 0;
		for(ALint i = 0; i < bufferCount; i++)
			alSourceUnqueueBuffers(m_source, 1, &tempBuffer);

		// Fill buffer queue
		for(std::size_t i = 0; i < m_buffers.size(); i++)
		{
			m_file->readSecondIntoBuffer(m_buffers[i].buffer, m_loop);
			alSourceQueueBuffers(m_source, 1, &m_buffers[i].buffer);
		}
	}

	void AudioStream::processStream()
	{
		bool regularStop = false;
		while(!m_requestStop)
		{

			// Ignore interrupts
			if(getStatus() == AudioSource::Stopped)
			{
				if(!regularStop)
					alSourcePlay(m_source);
				else
				{
					// TODO this won't cause the last buffer to be processed 
					printf("\nWOOP\n");
					// Grab the last offset as the sound reaches end of the file
					//ALfloat secOffset = 0;
					//alGetSourcef(m_source, AL_SEC_OFFSET, &secOffset);
					//m_playingOffset = Saurobyte::seconds(m_playingOffset.asSeconds() + secOffset);
					//break;
				}
			}


			ALint processedBuffers = 0;
			alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processedBuffers);

			// Process finished buffers
			while(processedBuffers > 0)
			{

				// Grab playing offset, usually around 1 (second) since that is the size of buffer chunks
				ALfloat secOffset = 0;
				alGetSourcef(m_source, AL_SEC_OFFSET, &secOffset);
				m_playingOffset = Saurobyte::seconds(m_playingOffset.asSeconds() + secOffset);

				// Pop used buffers from the queue and recycle them
				ALuint buffer;
				alSourceUnqueueBuffers(m_source, 1, &buffer);

				if(regularStop)
					return;

				// Read a 1s chunk of data
				if(!m_file->readSecondIntoBuffer(buffer, m_loop))
				{
					regularStop = true; // TODO fix this, not good enough
					//while(isPlaying())
					//{
						//Saurobyte::sleep(Saurobyte::milliseconds(10));
					//}
					//m_playingOffset = Time();
					//return;//m_requestStop = true; // End of file was reached
				}
				else
					alSourceQueueBuffers(m_source, 1, &buffer);


				--processedBuffers;
			}

			Saurobyte::sleep(Saurobyte::milliseconds(10));
		}

	}

	void AudioStream::setLooping(bool looping)
	{
		m_loop = looping;
	}
	void AudioStream::setOffset(Time offset)
	{
		if(isValid())
		{
			bool wasPlaying = isPlaying();
			stop();

			// Set reading offset of the file
			m_playingOffset = offset;
			m_file->setReadingOffset(offset.asSeconds());

			// Resume playing (if it was playing)
			if(wasPlaying)
				play();
		}
	}

	Time AudioStream::getOffset() const
	{
		if(isValid())
		{
			if(getStatus() != AudioSource::Stopped)
			{
				// Get offset from current buffer to get a real-time offset reading
				ALfloat seconds = 0;
				alGetSourcef(m_source, AL_SEC_OFFSET, &seconds);

				return Saurobyte::seconds(seconds + m_playingOffset.asSeconds());
			}
			else
				return m_playingOffset;
		}
		else
			return Time();
	}
	bool AudioStream::isLooping() const
	{
		return m_loop;
	}
};