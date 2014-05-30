
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

#include <Saurobyte/VideoDevice.hpp>
#include <Saurobyte/Logger.hpp>
#include <Saurobyte/Game.hpp>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>

namespace Saurobyte
{
	VideoDevice::VideoDevice(Game &game)
		:
		m_game(game)
	{
		// Initialzie GLEW
		glewExperimental = GL_TRUE;
		GLenum glewErrCode = glewInit();
		if(glewErrCode != GLEW_OK)
			SAUROBYTE_FATAL_LOG("Could not initialize GLEW: ", glewGetErrorString(glewErrCode));

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		// Read config data or use defaults
		LuaConfig &conf = game.getConfig();

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, conf.readBool("SauroConf.video.multisampling.on", false));
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, conf.readInt("SauroConf.video.multisampling.samples", 4));
	}
	VideoDevice::~VideoDevice()
	{

	}

	void VideoDevice::toggleWireFrame()
	{
		GLint polyMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polyMode);

		if(polyMode[1] == GL_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	void VideoDevice::resizeViewport(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);
	}
};