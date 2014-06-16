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


#ifndef SAUROBYTE_AUDIOSOURCE_HPP
#define SAUROBYTE_AUDIOSOURCE_HPP

#include <Saurobyte/NonCopyable.hpp>
#include <Saurobyte/Math/Vector3.hpp>
#include <Saurobyte/Time.hpp>
#include <memory>
#include <cstdint>

namespace Saurobyte
{
	namespace internal
	{
		class AudioFileImpl;
	}

	class AudioSource : public NonCopyable
	{
	public:

		enum AudioStatus
		{
			Playing,
			Paused,
			Stopped
		};


		//AudioSource();
		explicit AudioSource(std::unique_ptr<internal::AudioFileImpl> filePtr, std::uint32_t newSource);
		virtual ~AudioSource();

		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;

		void setPitch(float pitch);
		void setPosition(Vector3f position);
		void setVelocity(Vector3f velocity);
		void setDirection(Vector3f direction);
		void setRelativeToListener(bool relative);
		void setVolume(float volume);

		// AudioSources must implement their own functionality in the following
		virtual void setLooping(bool looping) = 0;
		virtual void setOffset(Time offset) = 0;

		virtual Time getOffset() const = 0;
		virtual bool isLooping() const = 0;

		const Time& getDuration() const;
		float getVolume() const;
		const Vector3f& getPosition() const;

		bool isPlaying() const;
		AudioStatus getStatus() const;

		// Whether or not the internal OpenAL source is valid
		bool isValid() const;


	protected:

		struct BufferWrapper
		{
			std::uint32_t buffer;
			BufferWrapper();
			~BufferWrapper();
		};

		typedef std::unique_ptr<internal::AudioFileImpl> AudioFilePtr;

		// OpenAL source handle
		std::uint32_t m_source;
		std::unique_ptr<internal::AudioFileImpl> m_file;

	private:

		friend class AudioDevice;

		// Whether or not the source was generated successfully
		bool m_isValidSource;

		/**
		 * Invalidates the source and returns it's internal OpenAL source identifier
		 * @return OpenAL source handle
		 */
		std::uint32_t invalidate();

		Time m_duration;

		Vector3f m_position;

	};
};

#endif