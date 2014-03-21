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


		struct GLAttribute
		{
			SDL_GLattr attribute;
			int value;
		};

	public:

		static OpenGLWindow* activeWindow;

		explicit OpenGLWindow(
			const std::string &windowTitle,
			std::size_t width,
			std::size_t height,
			Uint32 windowFlags,
			std::vector<GLAttribute> openGLAttributes,
			std::size_t maxFps = 60);
		~OpenGLWindow();
		void close();

		void setPosition(int x, int y);
		void setSize(std::size_t width, std::size_t height);
		void setFpsLimit(std::size_t fps);
		void setVsync(bool enabled);

		bool running();
		bool pollEvent(SDL_Event &event);
		void swapBuffers();

		// Utility function to quickly make this Window the globally accessible one
		void makeGlobal();

		void showPopup(Uint32 popupFlags, const std::string &title, const std::string &message);

		std::size_t getFps() const;
		float getDelta() const;

		std::size_t getWindowWidth() const;
		std::size_t getWindowHeight() const;

	};
};

#endif