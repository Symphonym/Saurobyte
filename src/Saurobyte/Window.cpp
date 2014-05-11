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
#include <SDL2/SDL.h>

namespace Saurobyte
{
	Window::Window(const std::string &title, int width, int height, const std::bitset<7> &windowFlags)
		:
		m_running(true)
	{
		Uint32 sdlFlags = 0;

		// OpenGL and High-dpi mode is enabled by default
		sdlFlags |= SDL_WINDOW_OPENGL;
		sdlFlags |= SDL_WINDOW_ALLOW_HIGHDPI;

		if(windowFlags.test(WindowFlags::Fullscreen))
			sdlFlags |= SDL_WINDOW_FULLSCREEN;
		if(windowFlags.test(WindowFlags::FullscreenDesktop))
			sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		if(windowFlags.test(WindowFlags::Borderless))
			sdlFlags |= SDL_WINDOW_BORDERLESS;
		if(windowFlags.test(WindowFlags::Resizable))
			sdlFlags |= SDL_WINDOW_RESIZABLE;
		if(windowFlags.test(WindowFlags::Maximized))
			sdlFlags |= SDL_WINDOW_MAXIMIZED;
		if(windowFlags.test(WindowFlags::Minimized))
			sdlFlags |= SDL_WINDOW_MINIMIZED;
		if(windowFlags.test(WindowFlags::Focused))
			sdlFlags |= SDL_WINDOW_INPUT_GRABBED;

		m_window = std::unique_ptr<internal::WindowImpl>(new internal::WindowImpl(title, width, height, sdlFlags));
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
	bool Window::running() const
	{
		return m_running;
	}
};