#include <Saurobyte/Game.hpp>
#include <Saurobyte/Message.hpp>

#include <Saurobyte/Systems/LuaSystem.hpp>

#include <Saurobyte/Lua/LuaEnv_Game.hpp>
#include <Saurobyte/Lua/LuaEnv_Entity.hpp>
#include <Saurobyte/Lua/LuaEnv_Input.hpp>
#include <Saurobyte/Lua/LuaEnv_Component.hpp>
#include <Saurobyte/Lua/LuaEnv_Scene.hpp>
#include <Saurobyte/Lua/LuaEnv_Audio.hpp>

namespace jl
{
	Game::Game(
		const std::string &name,
		int width,
		int height,
		std::vector<OpenGLWindow::OpenGLAttribute> glAttributes,
		OpenGLVersions glVersion)
		:
		m_entityPool(this),
		m_systemPool(this),
		m_scenePool(this),
		m_messageCentral(),
		m_luaEnvironment(),
		m_glWindow(nullptr),
		m_audioDevice()
	{
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
		};

		m_glWindow = new OpenGLWindow(name, width, height, SDL_WINDOW_SHOWN, glAttributes, 300);
		m_glWindow->setVsync(false);

		// Set default logging
		setLogging(GameLogging::Info_Error);

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
		delete m_glWindow;
	}

	void Game::setLogging(GameLogging logging)
	{
		// Start by disabling all logging
		SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);

		// Then enable specific logging features
		switch(logging)
		{
			case GameLogging::Debug:
				SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
			break;
			case GameLogging::Warning_Error:
				SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_WARN);
			break;
			case GameLogging::Info_Error:
				SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
				SDL_LogSetPriority(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);
			break;
			case GameLogging::None:
				SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
			break;
		}
	}

	void Game::gameLoop()
	{

		SDL_Event event;

		while(m_glWindow->running())
		{
			// Process frame start entity cleanup
			m_scenePool.frameCleanup();
			m_entityPool.frameCleanup();
			m_messageCentral.sendQueuedMessages();

			// Poll events
			while(m_glWindow->pollEvent(event))
			{
				if(event.type == SDL_QUIT)
					m_glWindow->close();
				else if(event.type == SDL_KEYDOWN)
				{
					sendMessage(createMessage<SDL_Event>("KeyDown", event));

					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						m_glWindow->close();
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

			m_glWindow->swapBuffers();
		}
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
	OpenGLWindow& Game::getWindow()
	{
		return *m_glWindow;
	}
};