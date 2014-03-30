#ifndef JL_AUDIOFILE_HPP
#define JL_AUDIOFILE_HPP

#include <AL/al.h>
#include <string>
#include <array>
#include <SndFile/sndfile.h>

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


		static const int StreamBufferCount = 3;
		// The amount of buffers used when streaming
		std::array<ALuint, StreamBufferCount> m_buffers;


		ReadingTypes m_readingType;
		AudioStatuses m_audioStatus;

		// Audio file data
		SF_INFO m_fileInfo;
		SNDFILE *m_file;
		std::string m_fileName;
		bool m_hasLoadedFile;

		SDL_Thread *m_updateThread;
		void updateData();
	public:

		AudioFile();
		~AudioFile();

		void readFile(const std::string &fileName, ReadingTypes readingType);

		void play();
		void pause();
		void stop();

		void setPlayingOffset(float offset);
		void setLooping(bool looping);

		// Returns the amount of seconds the file has played
		float getPlayingOffset() const;
		// Returns the length of the current audiofile, in seconds
		std::size_t getDuration() const;
		bool isPlaying() const;
	};
};

#endif