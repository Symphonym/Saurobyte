/*

	The MIT License (MIT)

	Copyright (c) 2014 by Jakob Larsson

	Permission is hereby granted, free of charge, to any person obtaining 
	a copy of this software and associated documentation files (the "Software"), 
	to deal in the Software without restriction, including without limitation the 
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
	sell copies of the Software, and to permit persons to whom the Software is 
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in 
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */


#ifndef SAUROBYTE_LOGGER_HPP
#define SAUROBYTE_LOGGER_HPP

#include <string>
#include <Saurobyte/Util.hpp>

#define SAUROBYTE_LOG_LINETOSTRING(x) #x
#define SAUROBYTE_LOG_LINETOVALUE(x) SAUROBYTE_LOG_LINETOSTRING(x)
#define SAUROBYTE_LOG_LOCATION __FILE__ ":" SAUROBYTE_LOG_LINETOVALUE(__LINE__) ": "

/**
 * Logs the specified message with respective priority
 * @param ... Arguments to concatenate into the log string
*/
#define SAUROBYTE_DEBUG_LOG(...) Saurobyte::Logger::log(Saurobyte::Logger::LogTypes::Fatal, __LINE__, __FILE__, Saurobyte::toStr(__VA_ARGS__))
#define SAUROBYTE_INFO_LOG(...) Saurobyte::Logger::log(Saurobyte::Logger::LogTypes::Info, __LINE__, __FILE__, Saurobyte::toStr(__VA_ARGS__))
#define SAUROBYTE_WARNING_LOG(...) Saurobyte::Logger::log(Saurobyte::Logger::LogTypes::Warning, __LINE__, __FILE__, Saurobyte::toStr(__VA_ARGS__))
#define SAUROBYTE_ERROR_LOG(...) Saurobyte::Logger::log(Saurobyte::Logger::LogTypes::Error, __LINE__, __FILE__, Saurobyte::toStr(__VA_ARGS__))
#define SAUROBYTE_FATAL_LOG(...) Saurobyte::Logger::log(Saurobyte::Logger::LogTypes::Fatal, __LINE__, __FILE__, Saurobyte::toStr(__VA_ARGS__))

namespace Saurobyte
{
	namespace Logger
	{
		enum class LogTypes
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal // Will throw an error and show a popup
		};
		enum LogStatus
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

		void setLogStatus(LogStatus status);
	};

};

/**
 * Throws an error and shows (if possible) an OS dependent message box with the error message as well as file and line number
 * @param  logMessage The message string to log
 */
//#define SAUROBYTE_THROW_ERROR(logMessage)  Saurobyte::displayAndThrowError(__LINE__, __FILE__, logMessage)

/**
 * Logs a message to the console
 * @param  formatMsg   [description]
 * @param  ... [description]
 */
//#define SAUROBYTE_DEBUG_LOG(formatMsg, ...) Saurobyte::debugLog(SAUROBYTE_LOG_LOCATION formatMsg, ##__VA_ARGS__)

// Log info, messages will display unless info logs are disabled

//#define SAUROBYTE_INFO_LOG(formatMsg, ...) Saurobyte::infoLog(formatMsg, ##__VA_ARGS__)

// Log warnings, messages will only display if warning logs are enabled
//#define SAUROBYTE_WARNING_LOG(formatMsg, ...) Saurobyte::warningLog(SAUROBYTE_LOG_LOCATION formatMsg, ##__VA_ARGS__)

// Log errors, messages will display unless error logs are disabled
//#define SAUROBYTE_ERROR_LOG(formatMsg, ...) Saurobyte::errorLog(SAUROBYTE_LOG_LOCATION formatMsg, ##__VA_ARGS__)


#endif