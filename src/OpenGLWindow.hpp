#ifndef JL_WINDOW_HPP
#define JL_WINDOW_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "FrameCounter.hpp"

namespace jl
{

	class OpenGLWindow
	{
	private:

		FrameCounter m_frameCounter;
		SDL_Window *m_window;
		SDL_GLContext m_glContext;

		bool m_isRunning;


	public:

		struct OpenGLAttribute
		{
			SDL_GLattr attribute;
			int value;
		};

		explicit OpenGLWindow(
			const std::string &windowTitle,
			std::size_t width,
			std::size_t height,
			Uint32 windowFlags,
			std::vector<OpenGLAttribute> openGLAttributes,
			std::size_t maxFps = 60);
		~OpenGLWindow();
		void close();

		void setPosition(int x, int y);
		void setSize(std::size_t width, std::size_t height);
		void setFpsLimit(std::size_t fps);
		void setVsync(bool enabled);
		void setGamma(float gamma);

		bool running();
		bool pollEvent(SDL_Event &event);
		void swapBuffers();

		void showPopup(Uint32 popupFlags, const std::string &title, const std::string &message);

		std::size_t getFps() const;
		float getDelta() const;

		std::size_t getWidth() const;
		std::size_t getHeight() const;

	};
};

#endif