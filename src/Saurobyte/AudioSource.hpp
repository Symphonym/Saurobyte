#ifndef JL_AUDIOSOURCE_HPP
#define JL_AUDIOSOURCE_HPP

#include <Saurobyte/NonCopyable.hpp>
#include <Saurobyte/Math/Vector3.hpp>
#include <Saurobyte/Time.hpp>
#include <memory>
#include <string>
#include <thread>
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

		void play();
		void pause();
		void stop();

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
		//unsigned int m_source;
		std::uint32_t m_source;
		std::unique_ptr<internal::AudioFileImpl> m_file;

		AudioStatus m_audioStatus;

	private:

		friend class AudioDevice;

		// Run by separate thread, keeps track of the audio source
		int updateData();

		virtual void onUpdate() {};

		virtual void onPlay() {};
		virtual void onPause() {};
		virtual void onStop() {};

		// Whether or not the source was generated successfully
		bool m_isValidSource;

		std::uint32_t invalidate();

		//SDL_Thread *m_thread;
		std::thread m_thread;
		Time m_duration;

		Vector3f m_position;

	};
};

#endif