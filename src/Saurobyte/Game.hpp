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
#include <Saurobyte/FrameCounter.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/LuaConfig.hpp>
#include <Saurobyte/Window.hpp>
#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/NonCopyable.hpp>
#include <string>

namespace Saurobyte
{

	class AudioDevice;
	class VideoDevice;
	class SAUROBYTE_API Game : public NonCopyable
	{
	public:


		/**
		 * Initializes the Game instance of the Saurobyte engine, of which there may be only one and creates an OpenGL window
		 * @param  title      Title of the window
		 * @param  width      Width of the window
		 * @param  height     Height of the window
		 * @param  windowMode Display mode of the window
		 */
		explicit Game(const std::string &title, int width, int height, Window::WindowModes windowMode);
		~Game();

		void start();
		void stop();

		void setFps(unsigned int fps);

		// Creating entities and scenes
		Entity& createEntity();
		Entity& createEntity(const std::string &templateName);
		Scene& createScene(const std::string &name);

		// Scene managing
		void changeScene(const std::string &sceneName);
		Scene* getActiveScene();

		// Message sending
		void sendMessage(const Message &message);
		void sendMessage(const std::string &messageName, Entity *entity = nullptr);
		template<typename TType> void sendMessage(const std::string &messageName, TType data, Entity *entity = nullptr)
		{
			sendMessage(MessageData<TType>(messageName, data, entity));
		};

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

		unsigned int getFps() const;
		float getDelta() const;

		EntityPool& getEntityPool();
		SystemPool& getSystemPool();
		ScenePool& getScenePool();

		MessageCentral& getMessageCentral();
		Window& getWindow();

		LuaEnvironment& getLua();
		LuaConfig& getConfig();

	private:

		EntityPool m_entityPool;
		SystemPool m_systemPool;
		ScenePool m_scenePool;

		FrameCounter m_frameCounter;

		std::unique_ptr<AudioDevice> m_audioDevice;
		std::unique_ptr<VideoDevice> m_videoDevice;

		LuaEnvironment m_luaEnvironment;
		LuaConfig m_luaConfig;

		MessageCentral m_messageCentral;
		Window m_window;

		void handleEvents();

		// Enforce one game instance
		static bool m_gameInstanceExists;
	};
};

#endif