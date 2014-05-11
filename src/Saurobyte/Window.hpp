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

#ifndef SAUROBYTE_WINDOW_HPP
#define SAUROBYTE_WINDOW_HPP

#include <Saurobyte/Math/Vector2.hpp>
#include <memory>
#include <string>
#include <bitset>

namespace Saurobyte
{
	namespace internal
	{
		class WindowImpl;
	};




	class Window
	{
	private:

		std::unique_ptr<internal::WindowImpl> m_window;

		bool m_running;

	public:

		enum WindowFlags
		{
			Fullscreen = 0, // Fullscreen window
			FullscreenDesktop = 1, // Fullscreen window with the current desktop resolution
			Borderless = 2, // Window without decoration (no border/titlebar)
			Resizable = 3, // Window is resizable
			Maximized = 4, // Maximized window
			Minimized = 5, // Minimized window
			Focused = 6 // Window has input focus
		};

		explicit Window(const std::string &title, int width, int height, const std::bitset<7> &windowFlags);

		void close();
		void show();
		void hide();

		void setTitle(const std::string &title);
		void setPosition(int x, int y);
		void setPosition(const Vector2i &position);
		void setSize(unsigned int width, unsigned int height);
		void setSize(const Vector2i &size);

		Vector2i getPosition() const;
		Vector2i getSize() const;
		bool running() const;
		// TODO setPos, getPOs etc
		// TODO Implement window with custom gl attribute enum and window flag enum
	};
};

#endif