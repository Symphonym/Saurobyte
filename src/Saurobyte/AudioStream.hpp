#ifndef JL_AUDIOSTREAM_HPP
#define JL_AUDIOSTREAM_HPP

#include <Saurobyte/AudioSource.hpp>

#include <string>
#include <array>
#include <memory>

namespace Saurobyte
{

	class AudioStream : public AudioSource
	{
	private:

		bool m_loop;

		// How many buffers to use when streaming
		static const unsigned char StreamBufferCount = 3;

		// Array of OpenAL buffer handles
		std::array<unsigned int, StreamBufferCount> m_buffers;

		// Handles the streaming
		virtual void onUpdate();

		void fillBuffers();

	public:

		//AudioStream();
		explicit AudioStream(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource);
		~AudioStream();

		void setStreamingFile(const std::string &filePath);

		virtual void setLooping(bool looping);
		virtual void setOffset(Time offset);

		virtual Time getOffset() const;
		bool isLooping() const;
	};
};

#endif