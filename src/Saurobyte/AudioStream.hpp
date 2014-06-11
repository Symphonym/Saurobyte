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

		// Array of OpenAL buffer handles
		std::array<AudioSource::BufferWrapper, 3> m_buffers;

		// Handles the streaming
		virtual void onUpdate();

	public:

		explicit AudioStream(AudioSource::AudioFilePtr audioPtr, std::uint32_t newSource);
		~AudioStream();

		virtual void setLooping(bool looping);
		virtual void setOffset(Time offset);

		virtual Time getOffset() const;
		bool isLooping() const;
	};
};

#endif