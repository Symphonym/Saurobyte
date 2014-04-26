#include "AudioListener.hpp"
#include <AL/al.h>

namespace jl
{
	namespace
	{
		Vector3f listenerPosition(0,0,0);
		Vector3f listenerDirection(0,0,-1);
		Vector3f listenerUpVector(0,1,0);

		float listenerVolume = 1.0f;
	};

	namespace AudioListener
	{
		void setVolume(float volume)
		{
			alListenerf(AL_GAIN, volume);
			listenerVolume = volume;

		}
		void setPosition(const Vector3f &position)
		{
			alListener3f(AL_POSITION, position.x, position.y, position.z);
			listenerPosition = position;
		}

		void setDirection(const Vector3f &direction)
		{
			listenerDirection = direction;
			float orientation[] = 
			{
				listenerDirection.x, // Direction
				listenerDirection.y,
				listenerDirection.z,
				listenerUpVector.x, // Up vector
				listenerUpVector.y,
				listenerUpVector.z,
			};
			alListenerfv(AL_ORIENTATION, orientation);
		}
		void setUpVector(const Vector3f &up)
		{
			listenerUpVector = up;
			setDirection(listenerDirection);
		}

		float getVolume()
		{
			return listenerVolume;
		}
		const Vector3f& getPosition()
		{
			return listenerPosition;
		}
	};
};