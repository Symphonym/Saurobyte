#include "OpenGLWindow.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Logger.hpp"

namespace jl
{

	OpenGLWindow::OpenGLWindow(
			const std::string &windowTitle,
			std::size_t width,
			std::size_t height,
			Uint32 windowFlags,
			std::vector<OpenGLAttribute> openGLAttributes,
			std::size_t maxFps)
		:
		m_frameCounter(),
		m_window(nullptr),
		m_glContext(),
		m_isRunning(true)
	{
		windowFlags |= SDL_WINDOW_OPENGL; // Make sure OpenGL is allowed in the Window

		// Set all desired OpenGL attributes for the context
		for(std::size_t i = 0; i < openGLAttributes.size(); i++)
			SDL_GL_SetAttribute(openGLAttributes[i].attribute, openGLAttributes[i].value);

		m_window = SDL_CreateWindow(
			windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);

		// Failed to create SDL window
		if(m_window == nullptr)
			JL_ERROR_LOG("SDL_CreateWindow failed. SDL_Error: %s", SDL_GetError());
		else
			JL_INFO_LOG("SDL window created: %s", windowTitle.c_str());

		m_frameCounter.limitFps(maxFps);

		// Create OpenGL context
		m_glContext = SDL_GL_CreateContext(m_window);

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum loadedGlew = glewInit();
		if(loadedGlew != GLEW_OK)
			JL_ERROR_LOG("GLEW initialization error: %s", glewGetErrorString(loadedGlew));

		JL_INFO_LOG("OpenGL: %s", glGetString(GL_VERSION));
		JL_INFO_LOG("OpenGL vendor: %s", glGetString(GL_VENDOR));
		JL_INFO_LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
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
			JL_WARNING_LOG("Vsync could not be enabled on this system. SDL_Error: %s", SDL_GetError());
	}
	void OpenGLWindow::setGamma(float gamma)
	{
		if(SDL_SetWindowBrightness(m_window, gamma))
			JL_ERROR_LOG("Couldn't set gamme value. SDL_Error: %s", SDL_GetError());

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

	std::size_t OpenGLWindow::getWidth() const
	{
		int width = 0;
		SDL_GetWindowSize(m_window, &width, NULL);
		return width;
	}
	std::size_t OpenGLWindow::getHeight() const
	{
		int height = 0;
		SDL_GetWindowSize(m_window, NULL, &height);
		return height;
	}
}