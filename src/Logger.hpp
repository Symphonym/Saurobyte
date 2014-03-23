#ifndef JL_LOGGER_HPP
#define JL_LOGGER_HPP

#include <SDL2/SDL_log.h>

#define JL_LOG_LINETOSTRING(x) #x
#define JL_LOG_LINETOVALUE(x) JL_LOG_LINETOSTRING(x)
#define JL_LOG_LOCATION __FILE__ ":" JL_LOG_LINETOVALUE(__LINE__) ": "

#define JL_DEBUG_LOG(formatMsg, ...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, JL_LOG_LOCATION formatMsg, ##__VA_ARGS__)
#define JL_INFO_LOG(formatMsg, ...) SDL_Log(formatMsg, ##__VA_ARGS__)
#define JL_WARNING_LOG(formatMsg, ...) SDL_LogWarn(SDL_LOG_CATEGORY_ERROR, JL_LOG_LOCATION formatMsg, ##__VA_ARGS__)
#define JL_ERROR_LOG(formatMsg, ...) SDL_LogError(SDL_LOG_CATEGORY_ERROR, JL_LOG_LOCATION formatMsg, ##__VA_ARGS__)

#endif