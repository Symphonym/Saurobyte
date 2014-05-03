#ifndef JL_AUDIOFILE_HPP
#define JL_AUDIOFILE_HPP

#include <AL/al.h>
#include <string>
#include <array>
#include <SndFile/sndfile.h>
#include <Saurobyte/Math.hpp>

class SDL_Thread;
namespace jl
{
	class AudioFile
	{
	public:

		// Different methods of reading an audiofile
		enum ReadingTypes
		{
			Stream,
			Load
		};

	private:

		enum AudioStatuses
		{
			Playing,
			Paused,
			Stopped
		};

		ALuint m_source;

		// The amount of buffers used when streaming
		static const int StreamBufferCount = 3;
		std::array<ALuint, StreamBufferCount> m_buffers;


		ReadingTypes m_readingType;
		AudioStatuses m_audioStatus;

		// Audio file data
		SF_INFO m_fileInfo;
		SNDFILE *m_file;
		std::string m_fileName;

		// If the audio file should loop, used instead of OpenAL loop
		// attribute since we don't want individual streaming buffers to loop.
		bool m_loop;
		bool m_hasLoadedFile;

		float m_playingOffset, m_duration;
		void fillBuffers();

		// Streaming data
		SDL_Thread *m_updateThread;
		void updateData();
	public:

		AudioFile();
		~AudioFile();

		// Reads the given file with the specified reading type, returns false if the file
		// could not be read.
		bool readFile(const std::string &fileName, ReadingTypes readingType = ReadingTypes::Load);

		void play();
		void pause();
		void stop();


		void setPosition(Vector3f position);
		void setPlayingOffset(float secondOffset);
		void setLooping(bool looping);
		void setPitch(float pitch);
		void setRelativeToListener(bool relative);
		void setVolume(float volume);

		// Returns the amount of seconds the file has played
		float getPlayingOffset() const;
		// Returns the length of the current audiofile, in seconds
		float getDuration() const;
		bool isPlaying() const;
	};
};

#endif