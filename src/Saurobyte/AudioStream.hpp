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



#ifndef SAUROBYTE_AUDIOSTREAM_HPP
#define SAUROBYTE_AUDIOSTREAM_HPP

#include <Saurobyte/AudioSource.hpp>
#include <array>
#include <thread>

namespace Saurobyte
{

	class AudioStream : public AudioSource
	{
	private:

		bool m_loop;
		bool m_requestStop;
		Time m_playingOffset;
		std::thread m_thread;

		// Array of OpenAL buffer handles
		std::array<AudioSource::BufferWrapper, 3> m_buffers;

		void processStream();
		void prepareStreaming();

	public:

		explicit AudioStream(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource);
		~AudioStream();

		virtual void play();
		virtual void pause();
		virtual void stop();

		virtual void setLooping(bool looping);
		virtual void setOffset(Time offset);

		virtual Time getOffset() const;
		bool isLooping() const;
	};
};

#endif