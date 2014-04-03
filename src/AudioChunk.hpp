#ifndef JL_AUDIOCHUNK_HPP
#define JL_AUDIOCHUNK_HPP

#include <string>
#include "AudioSource.hpp"

namespace jl
{
	class AudioChunk : public AudioSource
	{
	private:

		// OpenAL buffer handle
		unsigned int m_buffer;

		std::string m_fileName;

	public:

		explicit AudioChunk(unsigned int buffer, const std::string &fileName);

		void setBuffer(unsigned int buffer, const std::string &fileName);

		std::string getFileName() const;

	};
};

#endif