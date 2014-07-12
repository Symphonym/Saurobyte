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


#include <Saurobyte/AudioListener.hpp>
#include <al.h>

namespace Saurobyte
{
	namespace
	{
		Vector3f listenerPosition(0,0,0);
		Vector3f listenerDirection(0,0,-1);
		Vector3f listenerUpVector(0,1,0);

		float listenerVolume = 1.0f;
	};

	void AudioListener::setVolume(float volume)
	{
		alListenerf(AL_GAIN, volume);
		listenerVolume = volume;

	}
	void AudioListener::setPosition(const Vector3f &position)
	{
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		listenerPosition = position;
	}

	void AudioListener::setDirection(const Vector3f &direction)
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
	void AudioListener::setUpVector(const Vector3f &up)
	{
		listenerUpVector = up;
		setDirection(listenerDirection);
	}

	float AudioListener::getVolume()
	{
		return listenerVolume;
	}
	const Vector3f& AudioListener::getPosition()
	{
		return listenerPosition;
	}
};