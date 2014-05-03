#ifndef SAUROBYTE_LOGGER_HPP
#define SAUROBYTE_LOGGER_HPP

#include <string>
#include <Saurobyte/Util.hpp>

#define SAUROBYTE_LOG_LINETOSTRING(x) #x
#define SAUROBYTE_LOG_LINETOVALUE(x) SAUROBYTE_LOG_LINETOSTRING(x)
#define SAUROBYTE_LOG_LOCATION __FILE__ ":" SAUROBYTE_LOG_LINETOVALUE(__LINE__) ": "

namespace Saurobyte
{
	//namespace Logger
	//{
	//
		enum class LogTypes
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal // Will throw an error and show a popup
		};
		enum class LogStatus
		{
			Critical, // Enables logging only for CRITICAL messages (that terminates the program)
			Warning_Error, // Enables logging for WARNING and ERROR messages
			Info_Error, // Enables logging for INFO and ERROR messages 		 -- DEFAULT
			Debug, // Enables ALL logging
		};
	
		/**
		 * Logs the specified message to the OS dependant standard output
		 * @param logType    Logging type
		 * @param line       Line where this function was called
		 * @param file       File where this function was called
		 * @param logMessage The message to log
		 */
		void log(LogTypes logType, unsigned int line, const std::string &file, const std::string &logMessage);
		/**
		 * Utility function for logging, allowing for easy convertion of values to strings
		 * @param logType Logging type
		 * @param line    Line where this function was called
		 * @param file    File where this function was called
		 * @param args    Arguments that will be concatenated into a string and used for logging
		 */
		template<typename... TArgs> void logMsg(LogTypes logType, unsigned int line, const std::string &file, const TArgs&... args)
		{
			log(logType, line, file, toStr(args...));
		};

		void setLogStatus(LogStatus status);

		void debugLog(const std::string &logMessage, ...);
		void infoLog(const std::string &logMessage, ...);
		void warningLog(const std::string &logMessage, ...);
		void errorLog(const std::string &logMessage, ...);
		void displayAndThrowError(unsigned int line, const std::string &file, const std::string &logMessage);
	//};

};

/**
 * Throws an error and shows (if possible) an OS dependent message box with the error message as well as file and line number
 * @param  logMessage The message string to log
 */
#define SAUROBYTE_THROW_ERROR(logMessage)  Saurobyte::displayAndThrowError(__LINE__, __FILE__, logMessage)

/**
 * Logs a message to the console
 * @param  formatMsg   [description]
 * @param  ... [description]
 */
#define SAUROBYTE_DEBUG_LOG(formatMsg, ...) Saurobyte::debugLog(SAUROBYTE_LOG_LOCATION formatMsg, ##__VA_ARGS__)

// Log info, messages will display unless info logs are disabled
#define SAUROBYTE_I_LOG(...) Saurobyte::logMsg(Saurobyte::LogTypes::Info, __LINE__, __FILE__, __VA_ARGS__)
#define SAUROBYTE_INFO_LOG(formatMsg, ...) Saurobyte::infoLog(formatMsg, ##__VA_ARGS__)

// Log warnings, messages will only display if warning logs are enabled
#define SAUROBYTE_WARNING_LOG(formatMsg, ...) Saurobyte::warningLog(SAUROBYTE_LOG_LOCATION formatMsg, ##__VA_ARGS__)

// Log errors, messages will display unless error logs are disabled
#define SAUROBYTE_ERROR_LOG(formatMsg, ...) Saurobyte::errorLog(SAUROBYTE_LOG_LOCATION formatMsg, ##__VA_ARGS__)


#endif