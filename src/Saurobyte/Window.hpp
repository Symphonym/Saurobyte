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
#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/NonCopyable.hpp>
#include <memory>
#include <string>

namespace Saurobyte
{
	namespace internal
	{
		class WindowImpl;
	};

	class MessageCentral;
	class SAUROBYTE_API Window : public NonCopyable
	{
	private:

		std::unique_ptr<internal::WindowImpl> m_window;

		bool m_running;

	public:

		enum WindowModes
		{
			Normal = 0, // Normal unresizable window with titlebar
			Normless = 1, // Normal unresizable window without titlebar/borders
			Maximized = 2, // Normal window except it's maximized
			Maxless = 2, // Maximized window without titlebar/borders
			Fullscreen = 3, // Fullscreen window
		};

		/**
		 * Initializes a Window with the specified size and mode
		 * @param  title      Window title
		 * @param  width      Window width
		 * @param  height     Window height
		 * @param  windowMode Window display mode
		 */
		explicit Window(const std::string &title, int width, int height, WindowModes windowMode);
		~Window();

		/**
		 * Closes the window, effectively destroying it and its context
		 */
		void close();
		/**
		 * Shows the window
		 */
		void show();
		/**
		 * Hides the window (not minimizing)
		 */
		void hide();
		/**
		 * Swaps the back and front buffers of the window, updating the window with what OpenGL has rendered
		 */
		void swapBuffers();

		/**
		 * Changes the title of the window
		 * @param title Window title
		 */
		void setTitle(const std::string &title);
		/**
		 * Sets the position (screen coordinates) of the window
		 * @param x Window X coordinate
		 * @param y Window Y coordinate
		 */
		void setPosition(int x, int y);
		/**
		 * Sets the position (screen coordinates) of the window
		 * @param position Window coordinates
		 */
		void setPosition(const Vector2i &position);
		/**
		 * Sets the size of the window
		 * @param width  Window width
		 * @param height Window height
		 */
		void setSize(unsigned int width, unsigned int height);
		/**
		 * Sets the size of the window
		 * @param size Window size
		 */
		void setSize(const Vector2i &size);
		/**
		 * Changes the Vsync (Vertical Sync) of the window, locking frame rate to display refresh rate
		 * @param enabled Whether or not to enable Vsync
		 */
		void setVsync(bool enabled);
		/**
		 * Changes the gamma (brightness) of a window. The value 0.0 is dark and 1.0 is full brightness.
		 * @param gamma The gamma value, ranging 0.0 - 1.0
		 */
		void setGamma(float gamma);
		/**
		 * Changes the window mode of the window
		 * @param mode The new mode of the window
		 */
		void setMode(WindowModes mode);

		/**
		 * Moves the window to the monitor associated with the specified index, only works for fullscreen windows
		 * @param monitorIndex Index of the monitor
		 */
		void moveToMonitor(int monitorIndex);

		/**
		 * Gets the current position of the window
		 * @return Window coordinates
		 */
		Vector2i getPosition() const;
		/**
		 * Gets the current size of the window
		 * @return Window size
		 */
		Vector2i getSize() const;
		/**
		 * Gets the current title of the window
		 * @return Window title
		 */
		std::string getTitle() const;
		/**
		 * Checks if the window is still running (haven't been closed/destroyed)
		 * @return Whether or not the window is still running
		 */
		bool running() const;


		/**
		 * The unique ID that represents this window, mainly used internally
		 * @return The unique ID of this window
		 */
		unsigned int getID() const;
	};
};

#endif