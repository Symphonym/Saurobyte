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


#ifndef SAUROBYTE_AUDIO_CHUNK_HPP
#define SAUROBYTE_AUDIO_CHUNK_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/AudioSource.hpp>
#include <string>
#include <memory>

namespace Saurobyte
{

	// Audio buffer handle used for statically played sounds in order
	// to keep track of how many AudioChunks that are using the buffer.
	typedef std::shared_ptr<unsigned int> AudioBufferHandle;

	namespace internal
	{
		template<unsigned int bufferCount> class AudioImpl;
	};

	class AudioChunk : public AudioSource
	{
	private:

		// OpenAL buffer handle
		AudioBufferHandle m_buffer;

		std::string m_fileName;
		float m_duration;

	public:

		AudioChunk();
		explicit AudioChunk(unsigned int source, AudioBufferHandle buffer, const std::string &fileName);

		void setBuffer(AudioBufferHandle buffer, const std::string &filePath);

		virtual void setLooping(bool looping);
		virtual void setOffset(float secondOffset);

		virtual float getOffset() const;
		virtual float getDuration() const;
		virtual bool isLooping() const;

		std::string getFileName() const;

	};
};

#endif