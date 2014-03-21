#include "OpenGLWindow.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace jl
{

	OpenGLWindow* OpenGLWindow::activeWindow = nullptr;

	OpenGLWindow::OpenGLWindow(
			const std::string &windowTitle,
			std::size_t width,
			std::size_t height,
			Uint32 windowFlags,
			std::vector<GLAttribute> openGLAttributes,
			std::size_t maxFps)
		:
		m_frameCounter(),
		m_window(nullptr),
		m_glContext(),
		m_isRunning(true)
	{
		windowFlags |= SDL_WINDOW_OPENGL; // Allow for OpenGL in the Window

		// Set all desired OpenGL attributes for the context
		for(std::size_t i = 0; i < openGLAttributes.size(); i++)
			SDL_GL_SetAttribute(openGLAttributes[i].attribute, openGLAttributes[i].value);

		// Get center of screen
		int centerX = 100;
		int centerY = 100;

		SDL_Rect displayRect;
		if(!SDL_GetDisplayBounds(0, &displayRect))
		{
			centerX = displayRect.w/2 - width/2;
			centerY = displayRect.h/2 - height/2;
		}
		else
			SDL_Log("Could not find a default display to center window on. SDL_Error: %s", SDL_GetError());

		m_window = SDL_CreateWindow(windowTitle.c_str(), centerX, centerY, width, height, windowFlags);

		// Failed to create SDL window
		if(m_window == nullptr)
			SDL_Log("SDL_CreateWindow failed. SDL_Error: %s", SDL_GetError());
		else
			SDL_Log("SDL window created: %s", windowTitle.c_str());

		m_frameCounter.limitFps(maxFps);

		// Create OpenGL context
		m_glContext = SDL_GL_CreateContext(m_window);

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum loadedGlew = glewInit();
		if(loadedGlew != GLEW_OK)
			SDL_Log("GLEW initialization error: %s", glewGetErrorString(loadedGlew));

		SDL_Log("OpenGL: %s", glGetString(GL_VERSION));
		SDL_Log("OpenGL vendor: %s", glGetString(GL_VENDOR));
		SDL_Log("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

		// Make the most recent window the globally accessible one
		makeGlobal();
	}
	OpenGLWindow::~OpenGLWindow()
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
	}

	void OpenGLWindow::close()
	{
		m_isRunning = false;
	}

	bool OpenGLWindow::running()
	{
		m_frameCounter.update();
		return m_isRunning;
	}
	bool OpenGLWindow::pollEvent(SDL_Event &event)
	{
		return SDL_PollEvent(&event);
	}
	void OpenGLWindow::swapBuffers()
	{
		SDL_GL_SwapWindow(m_window);
	}

	void OpenGLWindow::makeGlobal()
	{
		OpenGLWindow::activeWindow = this;
		SDL_GL_MakeCurrent(m_window, m_glContext);
	}

	void OpenGLWindow::setPosition(int x, int y)
	{
		SDL_SetWindowPosition(m_window, x, y);
	}
	void OpenGLWindow::setSize(std::size_t width, std::size_t height)
	{
		SDL_SetWindowSize(m_window, width, height);
	}
	void OpenGLWindow::setFpsLimit(std::size_t fps)
	{
		m_frameCounter.limitFps(fps);
	}
	void OpenGLWindow::setVsync(bool enabled)
	{
		if(SDL_GL_SetSwapInterval(enabled ? 1 : 0) < 0)
			SDL_Log("Vsync could not be enabled on this system. SDL_Error: %s", SDL_GetError());
	}

	void OpenGLWindow::showPopup(Uint32 popupFlags, const std::string &title, const std::string &message)
	{
		SDL_ShowSimpleMessageBox(popupFlags, title.c_str(), message.c_str(), m_window);
	}

	std::size_t OpenGLWindow::getFps() const
	{
		return m_frameCounter.getFps();
	}
	float OpenGLWindow::getDelta() const
	{
		return m_frameCounter.getDelta();
	}

	std::size_t OpenGLWindow::getWindowWidth() const
	{
		int width = 0;
		SDL_GetWindowSize(m_window, &width, NULL);
		return width;
	}
	std::size_t OpenGLWindow::getWindowHeight() const
	{
		int height = 0;
		SDL_GetWindowSize(m_window, NULL, &height);
		return height;
	}
}