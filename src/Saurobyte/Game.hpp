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


#ifndef SAUROBYTE_GAME_HPP
#define SAUROBYTE_GAME_HPP

#include <Saurobyte/EntityPool.hpp>
#include <Saurobyte/SystemPool.hpp>
#include <Saurobyte/ScenePool.hpp>
#include <Saurobyte/MessageCentral.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/Window.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace Saurobyte
{
	enum class OpenGLProfiles
	{
		Compatibility, // OpenGL compatibility profile
		Core, // OpenGL Core profile
		ES // OpenGL ES profile
	};

	enum class OpenGLAttributes
	{
		RedBit, // Minimum number of bits for the red channel in the color buffer
		GreenBit, // Minimum number of bits for the green channel in the color buffer
		BlueBit, // Minimum number of bits for the blue channel in the color buffer
		AlphaBit, // Minimum number of bits for the alpha channel in the color buffer
		FrameBufferBit, // Minimum number of bits for the frame buffer size

		Doublebuffer, // Whether or not to use doublebuffering
		DepthBit, // Minimum number of bits in the depth buffer
		StencilBit, // Minimum number of bits in the stencil buffer
		AccumRedBit, // Minimum number of bits for the red channel in the accumulation buffer
		AccumGreenBit, // Minimum number of bits for the green channel in the accumulation buffer
		AccumBlueBit, // Minimum number of bits for the blue channel in the accumulation buffer
		AccumAlphaBit, // Minimum number of bits for the alpha channel in the accumulation buffer
		Stereo3D, // Whether or not to use Stereo3D

		MultisampleBuffers, // The number of buffers used for multisample anti-aliasing
		MultisampleSamples, // The number of samples used for multisampling
	};

	struct OpenGLAttribute
	{
		OpenGLAttributes attribute;
		int value;
	};

	class Game
	{
	private:

		EntityPool m_entityPool;
		SystemPool m_systemPool;
		ScenePool m_scenePool;
		MessageCentral m_messageCentral;
		LuaEnvironment m_luaEnvironment;

		//std::unique_ptr<OpenGLWindow> m_glWindow;

		AudioDevice m_audioDevice;

		std::unique_ptr<Window> m_window;

		// Enforce one game instance
		static bool m_gameInstanceExists;

	public:


		/**
		 * Initializes the Game instance of the Saurobyte engine, of which there may be only one
		 * @param  major      Major OpenGL version
		 * @param  minor      Minor OpenGL version
		 * @param  profile    OpenGL context profile
		 * @param  attributes Vector of OpenGL attributes that may be set
		 */
		explicit Game();
			//const std::string &name,
			//int width,
			//int height,
			//std::vector<OpenGLWindow::OpenGLAttribute> glAttributes = std::vector<OpenGLWindow::OpenGLAttribute>(),
			//OpenGLVersions glVersion = OpenGLVersions::Core_3_3);
		~Game();

		// TODO game constructor provides only OpenGL context data
		// TODO OpenGLWindow& createWindow(); 
		// TODO Hide OpenGLWindow interface with other window class or something
		// TODO Tidy up includes using window or game

		void gameLoop();

		// Creating entities and scenes
		Entity& createEntity();
		Entity& createEntity(const std::string &templateName);
		Scene& createScene(const std::string &name);

		// Scene managing
		void changeScene(const std::string &sceneName);
		Scene* getActiveScene();

		// Message sending
		void sendMessage(Message *message);
		void queueMessage(Message *message);

		// Running Lua scripts
		bool runScript(const std::string &filePath);
		/*template<typename TBaseType, typename TRealType = TBaseType> void exposeComponentToLua()
		{
			auto func = [] (lua_State *state) -> int
			{
				// First argument is self
				Entity *ent = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

				// All other args are optional parameters to constructor
				ent->addComponent<TBaseType>(new TRealType(state));

				return 0;
			};

			std::string funcName = "Add";


			lua_State *state = m_luaEnvironment.getRaw();

			// Grab component name
			TRealType tempComp = TRealType(state); // This doesn't have the args pushed, won't work
			funcName += tempComp.getName();

			const luaL_Reg funcs[] = 
			{
				{ funcName.c_str(), func },
				{ NULL, NULL }
			};


			// Grab entity metatable
			luaL_getmetatable(state, "jl.Entity");
			int metaTable = lua_gettop(state);

			luaL_setfuncs(state, funcs, 0);

			// Pop metatable value
			lua_pop(state, 1);
		};*/

		EntityPool& getEntityPool();
		SystemPool& getSystemPool();
		ScenePool& getScenePool();
		MessageCentral& getMessageCentral();
		LuaEnvironment& getLua();
		Window& getWindow();
	};
};

#endif