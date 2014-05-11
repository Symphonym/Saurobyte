#ifndef JL_MUTEXLOCK_HPP
#define JL_MUTEXLOCK_HPP

class SDL_mutex;
namespace Saurobyte
{
	class MutexLock
	{
	private:

		SDL_mutex *m_mutex;

	public:

		explicit MutexLock(SDL_mutex *mutex);
		~MutexLock();
	};
}

#endif