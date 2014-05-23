#include <Saurobyte/Game.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/Systems/LuaSystem.hpp>
#include <Saurobyte/Lua/LuaEnv_Game.hpp>
#include <Saurobyte/Lua/LuaEnv_Entity.hpp>
#include <Saurobyte/Lua/LuaEnv_Input.hpp>
#include <Saurobyte/Lua/LuaEnv_Component.hpp>
#include <Saurobyte/Lua/LuaEnv_Scene.hpp>
#include <Saurobyte/Lua/LuaEnv_Audio.hpp>
#include <Saurobyte/Logger.hpp>
#include <SDL2/SDL_opengl.h>

namespace Saurobyte
{
	bool Game::m_gameInstanceExists = false;

	Game::Game(const std::string &title, int width, int height, Window::WindowModes windowMode)
		:
		m_entityPool(this),
		m_systemPool(this),
		m_scenePool(this),
		m_messageCentral(),
		m_luaEnvironment(),
		m_audioDevice(),
		m_window(title, width, height, windowMode)
	{
		if(m_gameInstanceExists)
			SAUROBYTE_FATAL_LOG("Only one Game instance may exist!");
		else
			m_gameInstanceExists = true;

		// Set default logging
		Logger::setLogStatus(Logger::Info_Error);

		// Initialize libraries
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			SAUROBYTE_FATAL_LOG("SDL could not be initialized. SDL_Error: ", SDL_GetError());

		// Disable Vsync by default
		m_window.setVsync(false);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		// Use 4 samples if OpenGL enables multisampling
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum glewErrCode = glewInit();
		if(glewErrCode != GLEW_OK)
			SAUROBYTE_FATAL_LOG("Could not initialize GLEW: ", glewGetErrorString(glewErrCode));



		// Add the built in systems
		m_systemPool.addSystem(new LuaSystem(this));
		
		// Expose Lua API
		LuaEnv_Game::exposeToLua(this);
		LuaEnv_Entity::exposeToLua(this);
		LuaEnv_Input::exposeToLua(this);
		LuaEnv_Component::exposeToLua(this);
		LuaEnv_Scene::exposeToLua(this);
		LuaEnv_Audio::exposeToLua(this);
	}

	Game::~Game()
	{
		SDL_Quit();
	}

	void Game::gameLoop()
	{

		/*SDL_Event event;

		while(m_window->running())
		{
			// Process frame start entity cleanup
			m_scenePool.frameCleanup();
			m_entityPool.frameCleanup();
			m_messageCentral.sendQueuedMessages();

			// Poll events
			while(m_window->pollEvent(event))
			{
				if(event.type == SDL_QUIT)
					m_window->close();
				else if(event.type == SDL_KEYDOWN)
				{
					sendMessage(createMessage<SDL_Event>("KeyDown", event));

					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						m_window->close();
					else if(event.key.keysym.scancode == SDL_SCANCODE_R &&
						SDL_GetModState() & KMOD_LCTRL)
						sendMessage(createMessage("ReloadLua"));

				}
				else if(event.type == SDL_KEYUP)
					sendMessage(createMessage<SDL_Event>("KeyUp", event));
				else if(event.type == SDL_WINDOWEVENT)
				{
					if(event.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						glViewport(
							0,0,
							static_cast<int>(event.window.data1),
							static_cast<int>(event.window.data2));
					}
				}
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Process the systems and their entities
			m_systemPool.processSystems();
			
			glFlush();

			m_window->swapBuffers();
		}*/
	}

	Entity& Game::createEntity()
	{
		return m_entityPool.createEntity();
	}
	Entity& Game::createEntity(const std::string &templateName)
	{
		return m_entityPool.createEntity(templateName);
	}
	Scene& Game::createScene(const std::string &name)
	{
		return m_scenePool.createScene(name);
	}

	Scene* Game::getActiveScene()
	{
		return m_scenePool.getActiveScene();
	}
	void Game::changeScene(const std::string &sceneName)
	{
		m_scenePool.changeScene(sceneName);
	}

	void Game::sendMessage(Message *message)
	{
		m_messageCentral.sendMessage(message);
	}
	void Game::queueMessage(Message *message)
	{
		m_messageCentral.queueMessage(message);
	}

	bool Game::runScript(const std::string &filePath)
	{
		return m_luaEnvironment.runScript(filePath);
	}


	EntityPool& Game::getEntityPool()
	{
		return m_entityPool;
	}
	SystemPool& Game::getSystemPool()
	{
		return m_systemPool;
	}
	ScenePool& Game::getScenePool()
	{
		return m_scenePool;
	}
	MessageCentral& Game::getMessageCentral()
	{
		return m_messageCentral;
	}
	LuaEnvironment& Game::getLua()
	{
		return m_luaEnvironment;
	}
	Window& Game::getWindow()
	{
		return m_window;
	}
};