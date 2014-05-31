
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
#include <Saurobyte/Window.hpp>
#include <Saurobyte/Color.hpp>
#include <memory>

namespace Saurobyte
{
	class Game;
	class SAUROBYTE_API VideoDevice
	{
	public:

		~VideoDevice();

		/**
		 * Clears the OpenGL color, depth and stencil buffers depending on what features are enabled
		 */
		void clearBuffers();

		/**
		 * Toggles wire frame mode rendering
		 */
		static void toggleWireFrame();

		/**
		 * Resizes the OpenGL viewport to match that of the specified arguments, this is called automatically internally
		 * @param width  The width of the viewport
		 * @param height The height of the viewport
		 */
		static void setViewport(unsigned int width, unsigned int height);

		/**
		 * Sets the default background color used by OpenGL
		 * @param color The color to use
		 */
		static void setBackgroundColor(const Color &color);

		// TODO create shader n stuff
		// 
		Window& getWindow();

	private:

		// Only the game class can create a VideoDevice
		friend class Game;

		VideoDevice(
			Game &game,
			const std::string &windowTitle,
			unsigned int windowWidth,
			unsigned int windowHeight,
			Window::WindowModes windowMode = Window::Normal);

		Game &m_game;

		std::unique_ptr<Window> m_window;
	};
}

#endif