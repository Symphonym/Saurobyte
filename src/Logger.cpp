#include "Logger.hpp"
#include <SDL2/SDL_messagebox.h>
#include <stdexcept>

namespace jl
{
	void displayAndThrowError(const std::string &logMessage)
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Fatal error",
			logMessage.c_str(),
			NULL);

		throw std::runtime_error(logMessage);
	};
};