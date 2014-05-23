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

#include <Saurobyte/WindowImpl.hpp>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
{
	namespace internal
	{
		WindowImpl::WindowImpl(const std::string &title, int width, int height, Uint32 flags)
		{
			openGLContext = SDL_GL_CreateContext(window);

			if(openGLContext == NULL)
				SAUROBYTE_FATAL_LOG("Could not create OpenGL context: ", SDL_GetError());
			else
			{
				window = SDL_CreateWindow(
					title.c_str(),
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					width,
					height,
					flags);

				if(window == NULL)
					SAUROBYTE_FATAL_LOG("Could not create window by the title '", title, "': ", SDL_GetError());

				SDL_GL_MakeCurrent(window, openGLContext);
			}
		}
		WindowImpl::~WindowImpl()
		{
			if(window != NULL)
				SDL_DestroyWindow(window);
			SDL_GL_DeleteContext(openGLContext);
		}

		void WindowImpl::close()
		{
			if(window != NULL)
			{
				SDL_DestroyWindow(window);
				window = NULL;
			}
		}

	};
};