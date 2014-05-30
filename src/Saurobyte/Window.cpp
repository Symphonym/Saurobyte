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

#include <Saurobyte/Window.hpp>
#include <Saurobyte/WindowImpl.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/MessageCentral.hpp>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

namespace Saurobyte
{
	Window::Window(const std::string &title, int width, int height, WindowModes windowMode)
		:
		m_window(nullptr),
		m_running(true)
	{
		Uint32 sdlFlags = 0;

		// OpenGL and High-dpi mode is enabled by default
		sdlFlags |= SDL_WINDOW_OPENGL;
		sdlFlags |= SDL_WINDOW_ALLOW_HIGHDPI;

		if(windowMode == WindowModes::Normless ||
			windowMode == WindowModes::Maxless)
		{
			sdlFlags |= SDL_WINDOW_BORDERLESS;
		}

		if(windowMode == WindowModes::Maxless ||
			windowMode == WindowModes::Maximized)
		{
			sdlFlags |= SDL_WINDOW_MAXIMIZED;
		}

		if(windowMode == WindowModes::Fullscreen)
		{
			sdlFlags |= SDL_WINDOW_FULLSCREEN;
		}

		m_window = std::unique_ptr<internal::WindowImpl>(new internal::WindowImpl(title, width, height, sdlFlags));
	}
	Window::~Window()
	{
		
	}

	
	void Window::close()
	{
		m_window->close();
		m_running = false;
	}
	void Window::show()
	{
		SDL_ShowWindow(m_window->window);
	}
	void Window::hide()
	{
		SDL_HideWindow(m_window->window);
	}
	void Window::swapBuffers()
	{
		SDL_GL_SwapWindow(m_window->window);
	}

	void Window::setTitle(const std::string &title)
	{
		SDL_SetWindowTitle(m_window->window, title.c_str());
	}
	void Window::setPosition(int x, int y)
	{
		SDL_SetWindowPosition(m_window->window, x, y);
	}
	void Window::setPosition(const Vector2i &position)
	{
		SDL_SetWindowPosition(m_window->window, position.x, position.y);
	}
	void Window::setSize(unsigned int width, unsigned int height)
	{
		SDL_SetWindowSize(m_window->window, width, height);
	}
	void Window::setSize(const Vector2i &size)
	{
		SDL_SetWindowSize(m_window->window, size.x, size.y);
	}
	void Window::setVsync(bool enabled)
	{
		if(SDL_GL_SetSwapInterval(enabled ? 1 : 0) < 0)
			SAUROBYTE_WARNING_LOG("Vsync could not be enabled on this system. SDL_Error: ", SDL_GetError());
	}
	void Window::setGamma(float gamma)
	{
		if(SDL_SetWindowBrightness(m_window->window, gamma))
			SAUROBYTE_WARNING_LOG("Couldn't set gamma value. SDL_Error: ", SDL_GetError());
	}
	void Window::setMode(WindowModes mode)
	{
		// Modify window borders
		if(mode == WindowModes::Normless ||
			mode == WindowModes::Maxless)
			SDL_SetWindowBordered(m_window->window, SDL_FALSE);
		else
			SDL_SetWindowBordered(m_window->window, SDL_TRUE);

		// Modify window maximization
		if(mode == WindowModes::Maxless ||
			mode == WindowModes::Maximized)
			SDL_MaximizeWindow(m_window->window);
		else
			SDL_RestoreWindow(m_window->window);

		// Modify window fullscreen mode
		if(mode == WindowModes::Fullscreen)
			SDL_SetWindowFullscreen(m_window->window, SDL_WINDOW_FULLSCREEN);
		else
			SDL_SetWindowFullscreen(m_window->window, 0);
	}

	void Window::moveToMonitor(int monitorIndex)
	{
		SDL_DisplayMode mode;
		if(SDL_GetCurrentDisplayMode(monitorIndex, &mode))
		{
			SAUROBYTE_INFO_LOG("Moving window to monitor ", monitorIndex, "(", SDL_GetDisplayName(monitorIndex),")");
			SDL_SetWindowDisplayMode(m_window->window, &mode);
		}
		else
			SAUROBYTE_ERROR_LOG("Attempted to move window to invalid monitor index. SDL_Error: ", SDL_GetError());
	}


	Vector2i Window::getPosition() const
	{
		Vector2i pos;
		SDL_GetWindowPosition(m_window->window, &pos.x, &pos.y);
		return pos;
	}
	Vector2i Window::getSize() const
	{
		Vector2i size;
		SDL_GetWindowSize(m_window->window, &size.x, &size.y);
		return size;
	}
	std::string Window::getTitle() const
	{
		std::string title(SDL_GetWindowTitle(m_window->window));
		return title;
	}
	bool Window::running() const
	{
		return m_running;
	}

	unsigned int Window::getID() const
	{
		return SDL_GetWindowID(m_window->window);
	}
};