#ifndef JL_AUDIOSOURCE_HPP
#define JL_AUDIOSOURCE_HPP

#include <string>
#include "Math.hpp"

class SDL_Thread;
namespace jl
{
	class AudioSource
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

		unsigned int m_lastUsageTick;

		SDL_Thread *m_thread;

	protected:

		// OpenAL source handle
		unsigned int m_source;

		AudioStatus m_audioStatus;

	public:

		explicit AudioSource();
		virtual ~AudioSource();

		void play();
		void pause();
		void stop();

		void setPitch(float pitch);
		void setPosition(Vector3f position);
		void setVelocity(Vector3f velocity);
		void setDirection(Vector3f direction);
		void setRelativeToListener(bool relative);

		// This is virtual since all audio sources are not using the same
		// methods to loop, i.e streams.
		virtual void setLooping(bool looping);

		bool isPlaying() const;
	};
};

#endif