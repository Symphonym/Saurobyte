#include <Saurobyte/Game.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/OpenGLWindow.hpp>
#include <Saurobyte/WindowImpl.hpp>

#include <Saurobyte/Systems/LuaSystem.hpp>

#include <Saurobyte/Lua/LuaEnv_Game.hpp>
#include <Saurobyte/Lua/LuaEnv_Entity.hpp>
#include <Saurobyte/Lua/LuaEnv_Input.hpp>
#include <Saurobyte/Lua/LuaEnv_Component.hpp>
#include <Saurobyte/Lua/LuaEnv_Scene.hpp>
#include <Saurobyte/Lua/LuaEnv_Audio.hpp>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
{
	bool Game::m_gameInstanceExists = false;

	Game::Game(
		/*const std::string &name,
		int width,
		int height,
		std::vector<OpenGLWindow::OpenGLAttribute> glAttributes,
		OpenGLVersions glVersion*/)
		:
		m_entityPool(this),
		m_systemPool(this),
		m_scenePool(this),
		m_messageCentral(),
		m_luaEnvironment(),
		m_audioDevice()
	{
		if(m_gameInstanceExists)
			SAUROBYTE_FATAL_LOG("Only one Game instance may exist!");
		else
			m_gameInstanceExists = false;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		// Expose Lua API
	/*	LuaEnv_Game::exposeToLua(this);
		LuaEnv_Entity::exposeToLua(this);
		LuaEnv_Input::exposeToLua(this);
		LuaEnv_Component::exposeToLua(this);
		LuaEnv_Scene::exposeToLua(this);
		internal::LuaEnv_Audio::exposeToLua(&m_luaEnvironment);*/

		/*
		switch(glVersion)
		{
			// Core 3.3 is default
			default:
			case OpenGLVersions::Core_3_3:
				glAttributes.push_back({SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE});
				glAttributes.push_back({SDL_GL_CONTEXT_MAJOR_VERSION, 3});
				glAttributes.push_back({SDL_GL_CONTEXT_MINOR_VERSION, 3});
			break;
			case OpenGLVersions::Core_4_3:
				glAttributes.push_back({SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE});
				glAttributes.push_back({SDL_GL_CONTEXT_MAJOR_VERSION, 4});
				glAttributes.push_back({SDL_GL_CONTEXT_MINOR_VERSION, 3});
			break;
			/*case OpenGLVersions::ES_3_3:
				glAttributes.push_back({SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES});
				glAttributes.push_back({SDL_GL_CONTEXT_MAJOR_VERSION, 3});
				glAttributes.push_back({SDL_GL_CONTEXT_MINOR_VERSION, 3});
			break;
			case OpenGLVersions::ES_4_3:
				glAttributes.push_back({SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES});
				glAttributes.push_back({SDL_GL_CONTEXT_MAJOR_VERSION, 4});
				glAttributes.push_back({SDL_GL_CONTEXT_MINOR_VERSION, 3});
			break;*/
		/*};

		m_window = std::unique_ptr<OpenGLWindow>(new OpenGLWindow(name, width, height, SDL_WINDOW_SHOWN, glAttributes, 300));
		m_window->setVsync(false);

		// Set default logging
		Logger::setLogStatus(Logger::Info_Error);

		// Add the built in systems
		m_systemPool.addSystem(new LuaSystem(this));

		// Expose Lua API
		LuaEnv_Game::exposeToLua(this);
		LuaEnv_Entity::exposeToLua(this);
		LuaEnv_Input::exposeToLua(this);
		LuaEnv_Component::exposeToLua(this);
		LuaEnv_Scene::exposeToLua(this);
		LuaEnv_Audio::exposeToLua(this);*/
	}

	Game::~Game()
	{
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
		m_luaEnvironment.runScript(filePath);
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
		return *m_window;
	}
};