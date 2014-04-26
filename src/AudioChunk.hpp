#ifndef JL_AUDIOCHUNK_HPP
#define JL_AUDIOCHUNK_HPP

#include <string>
#include <memory>
#include "AudioSource.hpp"

namespace jl
{

	// Audio buffer handle used for statically played sounds in order
	// to keep track of how many AudioChunks that are using the buffer.
	typedef std::shared_ptr<unsigned int> AudioBufferHandle;

	class AudioChunk : public AudioSource
	{
	private:

		// OpenAL buffer handle
		AudioBufferHandle m_buffer;

		std::string m_fileName;
		float m_duration;

	public:

		AudioChunk();
		explicit AudioChunk(unsigned int source, AudioBufferHandle buffer, const std::string &fileName);

		void setBuffer(AudioBufferHandle buffer, const std::string &filePath);

		virtual void setLooping(bool looping);
		virtual void setOffset(float secondOffset);

		virtual float getOffset() const;
		virtual float getDuration() const;
		virtual bool isLooping() const;

		std::string getFileName() const;

	};
};

#endif