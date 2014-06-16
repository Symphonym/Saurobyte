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


		virtual ~AudioSource();

		virtual void play() = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;

		/**
		 * Sets the pitch of the source
		 * @param pitch The pitch, must be between 0.5-2.0
		 */
		void setPitch(float pitch);
		/**
		 * Sets the 3D position of the sound, only has an effect if the sound is relative to the listener
		 * @param position Vector of the position
		 */
		void setPosition(Vector3f position);
		/**
		 * Sets the velocity of the sound, used to accurately depict the sound in a 3D environment
		 * @param velocity Vector of the velocity
		 */
		void setVelocity(Vector3f velocity);
		/**
		 * Sets the direction of the sound, used to accurately depict the sound in a 3D environment
		 * @param direction Vector of the direction
		 */
		void setDirection(Vector3f direction);
		/**
		 * Sets whether or not the sound should be positional; relative to the listener in a 3D environment
		 * @param relative Whether or not the sound should be positional
		 */
		void setRelativeToListener(bool relative);
		/**
		 * Sets the volume of the sound
		 * @param volume Sound volume, must be between 0.0-1.0
		 */
		void setVolume(float volume);

		virtual void setLooping(bool looping) = 0;
		virtual void setOffset(Time offset) = 0;

		virtual Time getOffset() const = 0;
		virtual bool isLooping() const = 0;

		/**
		 * Returns the full length of the underlying audio file of the sound, if there is one
		 * @return The audio file duration as a Time object
		 */
		const Time& getDuration() const;
		/**
		 * Returns the volume of the sound
		 * @return Sound volume
		 */
		float getVolume() const;
		/**
		 * Returns the position of the sound
		 * @return Sound position
		 */
		const Vector3f& getPosition() const;

		/**
		 * Checks if the sound is currently playing; meaning that getStatus() == AudioStatus::Playing
		 * @return True if the sound is playing, false otherwise
		 */
		bool isPlaying() const;
		/**
		 * Returns the current status of the sound
		 * @return Status of the sound; Stopped, Paused or Playing
		 */
		AudioStatus getStatus() const;

		/**
		 * Checks if the underlying OpenAL source is valid. As if audio overflow occurs, less important sounds will be overwritten
		 * @return True if the OpenAL source is valid, false otherwise
		 */
		bool isValid() const;


	protected:

		/**
		 * BufferWrapper
		 *
		 * Simple wrapper used to RAII wrap OpenAL buffers
		 */
		struct BufferWrapper
		{
			std::uint32_t buffer;
			BufferWrapper();
			~BufferWrapper();
		};

		/**
		 * Initializes a sound from an opened file and an OpenAL source handle
		 * @param  filePtr   An opened file
		 * @param  newSource OpenAL source handle
		 */
		explicit AudioSource(std::unique_ptr<internal::AudioFileImpl> filePtr, std::uint32_t newSource);

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