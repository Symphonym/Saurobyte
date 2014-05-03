#include <Saurobyte/Logger.hpp>
#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/Util.hpp>
#include <SDL2/SDL_messagebox.h>
#include <SDL2/SDL_log.h>
#include <stdexcept>
#include <cstdarg>
#include <sstream>

namespace Saurobyte
{
	void log(LogTypes logType, unsigned int line, const std::string &file, const std::string &logMessage)
	{
		SDL_LogPriority priority = SDL_LOG_PRIORITY_VERBOSE; // We don't use the verbose priority
		int category = SDL_LOG_CATEGORY_APPLICATION;

		// Convert from Saurobyte priority to SDL prority
		switch(logType)
		{
			case LogTypes::Debug: priority = SDL_LOG_PRIORITY_DEBUG; break;
			case LogTypes::Info: priority = SDL_LOG_PRIORITY_INFO; break;
			case LogTypes::Warning: priority = SDL_LOG_PRIORITY_WARN; break;
			case LogTypes::Error: priority = SDL_LOG_PRIORITY_ERROR; break;
			case LogTypes::Fatal: priority = SDL_LOG_PRIORITY_CRITICAL; break;
		}

		// Do the same for categories
		switch(logType)
		{
			case LogTypes::Debug:
			case LogTypes::Info:
				category = SDL_LOG_CATEGORY_APPLICATION;
				break;
			case LogTypes::Warning:
			case LogTypes::Error:
			case LogTypes::Fatal:
				category = SDL_LOG_CATEGORY_ERROR;
				break;
		}

		if(priority != SDL_LOG_PRIORITY_VERBOSE)
		{
			std::string logMsg = toStr(file, " (", line, "): ", logMessage).c_str();
			SDL_LogMessage(category, priority, logMsg.c_str());
		}

		// Throw error and show popup if it's a fatal error
		if(logType == LogTypes::Fatal)
		{
			std::string popupMsg = toStr("File: ", file, "\n", "Line: ", line, "\n\n", logMessage);
			std::string titleMsg = toStr(SAUROBYTE_LIBRARY_NAME, " - Fatal error");

			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				titleMsg.c_str(),
				popupMsg.c_str(),
				NULL);

			throw std::runtime_error(logMessage);
		}
	}

	void setLogStatus(LogStatus status)
	{
		// Start by disabling all unnecessary logging 
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);

		// Then enable specific logging features
		switch(status)
		{
			case LogStatus::Debug:
				SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
			break;
			case LogStatus::Warning_Error:
				SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN);
			break;
			case LogStatus::Info_Error:
				SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
				SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);
			break;
			case LogStatus::Critical:
				SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
			break;
		}
	}

	void debugLog(const std::string &logMessage, ...)
	{
		va_list argList;
		va_start(argList, logMessage);

		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, logMessage.c_str(), argList);

		va_end(argList);

	}
	void infoLog(const std::string &logMessage, ...)
	{
		va_list argList;
		va_start(argList, logMessage);

		SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, logMessage.c_str(), argList);

		va_end(argList);
	}
	void warningLog(const std::string &logMessage, ...)
	{
		va_list argList;
		va_start(argList, logMessage);

		SDL_LogMessageV(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN, logMessage.c_str(), argList);

		va_end(argList);
	}
	void errorLog(const std::string &logMessage, ...)
	{
		va_list argList;
		va_start(argList, logMessage);

		SDL_LogMessageV(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, logMessage.c_str(), argList);

		va_end(argList);
	}
	void displayAndThrowError(unsigned int line, const std::string &file, const std::string &logMessage)
	{
		std::stringstream errorMsg;
		errorMsg << "File: " << file << "\n" << "Line: " << line << "\n\n" << logMessage;

		std::stringstream titleMsg;
		titleMsg << SAUROBYTE_LIBRARY_NAME << " - Fatal error";

		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			titleMsg.str().c_str(),
			errorMsg.str().c_str(),
			NULL);

		throw std::runtime_error(logMessage);
	};
};