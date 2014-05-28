
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

#ifndef SAUROBYTE_VIDEO_DEVICE_HPP
#define SAUROBYTE_VIDEO_DEVICE_HPP

#include <Saurobyte/ApiDefines.hpp>

namespace Saurobyte
{
	class Game;
	class SAUROBYTE_API VideoDevice
	{
	public:

		~VideoDevice();

		/**
		 * Toggles wire frame mode rendering
		 */
		static void toggleWireFrame();

		//create shader n stuff

	private:

		// Only the game class can create a VideoDevice
		friend class Game;

		VideoDevice(Game &game);

		Game &m_game;
	};
}

#endif