#ifndef JL_AUDIOSOURCE_HPP
#define JL_AUDIOSOURCE_HPP

#include <string>
#include <Saurobyte/NonCopyable.hpp>
#include <Saurobyte/Math/Vector3.hpp>

class SDL_Thread;
namespace Saurobyte
{
	class AudioSource : public NonCopyable
	{
	public:
		enum AudioStatus
		{
			Playing,
			Paused,
			Stopped
		};
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

		SDL_Thread *m_thread;

		Vector3f m_position;

	protected:

		// OpenAL source handle
		unsigned int m_source;

		AudioStatus m_audioStatus;

	public:

		AudioSource();
		explicit AudioSource(unsigned int source);
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
		virtual void setOffset(float secondOffset) = 0;

		virtual float getOffset() const = 0;
		virtual float getDuration() const = 0;
		virtual bool isLooping() const = 0;

		float getVolume() const;
		const Vector3f& getPosition() const;

		bool isPlaying() const;

		// Whether or not the internal OpenAL source is valid
		bool isValid() const;
	};
};

#endif