#include <Saurobyte/MutexLock.hpp>
#include <SDL2/SDL_mutex.h>

namespace jl
{
	MutexLock::MutexLock(SDL_mutex *mutex)
		:
		m_mutex(mutex)
	{
		SDL_LockMutex(m_mutex);
	}
	MutexLock::~MutexLock()
	{
		SDL_UnlockMutex(m_mutex);
	}
};