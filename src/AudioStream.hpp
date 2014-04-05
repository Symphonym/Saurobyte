#ifndef JL_AUDIOSTREAM_HPP
#define JL_AUDIOSTREAM_HPP

#include "AudioSource.hpp"

#include <string>
#include <array>
#include <SndFile/sndfile.h>

namespace jl
{
	class AudioStream : public AudioSource
	{
	private:

		// File data
		SNDFILE *m_file;
		SF_INFO m_fileInfo;
		std::string m_fileName;

		bool m_loop;
		float m_duration;

		// How many buffers to use when streaming
		static const unsigned char StreamBufferCount = 3;

		// Array of OpenAL buffer handles
		std::array<unsigned int, StreamBufferCount> m_buffers;

		// Handles the streaming
		virtual void onUpdate();

		void fillBuffers();

	public:

		explicit AudioStream(const std::string &filePath);
		~AudioStream();

		virtual void setLooping(bool looping);
		virtual void setOffset(float secondOffset);
	};
};

#endif