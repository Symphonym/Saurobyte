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
#include <Saurobyte/Event.hpp>
#include <Saurobyte/InputImpl.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/VideoDevice.hpp>

namespace Saurobyte
{
	bool Game::m_gameInstanceExists = false;

	Game::Game(const std::string &title, unsigned int width, unsigned int height, Window::WindowModes windowMode)
		:
		m_entityPool(this),
		m_systemPool(this),
		m_scenePool(this),
		m_frameCounter(),
		m_audioDevice(nullptr),
		m_videoDevice(nullptr),
		m_luaEnvironment(),
		m_luaConfig(m_luaEnvironment),
		m_messageCentral()
	{
		if(m_gameInstanceExists)
			SAUROBYTE_FATAL_LOG("Only one Game instance may exist!");
		else
			m_gameInstanceExists = true;


		// Initialize libraries
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
			SAUROBYTE_FATAL_LOG("SDL could not be initialized. SDL_Error: ", SDL_GetError());

		// Set default logging
		Logger::setLogStatus(Logger::Info_Error);

		if(!m_luaConfig.load("./sauroConf.lua"))
			SAUROBYTE_WARNING_LOG("No config file provided!");

		// Default FPS to 300
		m_frameCounter.limitFps(300);

		m_videoDevice = std::unique_ptr<VideoDevice>(new VideoDevice(*this, title, width, height, windowMode));
		m_audioDevice = std::unique_ptr<AudioDevice>(new AudioDevice());

		// TODO init devices here when log is loaded


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

	bool Game::handleEvents()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				// Broadcast window events
				case SDL_WINDOWEVENT:
					if(event.window.windowID == getWindow().getID())
					{
						switch (event.window.event)
						{
							case SDL_WINDOWEVENT_SHOWN:
								sendMessage<WindowEvent>("WindowShow", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_HIDDEN:
								sendMessage<WindowEvent>("WindowHide", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_ENTER:
								sendMessage<WindowEvent>("WindowMouseEnter", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_LEAVE:
								sendMessage<WindowEvent>("WindowMouseLeave", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_FOCUS_GAINED:
								sendMessage<WindowEvent>("WindowGainFocus", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_FOCUS_LOST:
								sendMessage<WindowEvent>("WindowLostFocus", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_CLOSE:
								sendMessage<WindowEvent>("WindowClose", WindowEvent(getWindow()));
								break;
							case SDL_WINDOWEVENT_RESIZED:
								{
									unsigned int newWidth = static_cast<unsigned int>(event.window.data1);
									unsigned int newHeight = static_cast<unsigned int>(event.window.data2);

									VideoDevice::setViewport(newWidth, newHeight);

									sendMessage<WindowSizeEvent>("WindowMove", 
										WindowSizeEvent(getWindow(), newWidth, newHeight));
								}
								break;
							case SDL_WINDOWEVENT_MOVED:
								sendMessage<WindowEvent>("WindowMove", 
									WindowMoveEvent(
										getWindow(),
										static_cast<int>(event.window.data1),
										static_cast<int>(event.window.data2)));
								break;
						}
					}
					break;

				// Broadcast keyboard events
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if(event.key.windowID == getWindow().getID())
					{
						sendMessage<KeyEvent>(
							event.type == SDL_KEYDOWN ? "KeyDown" : "KeyUp",
							KeyEvent(
								internal::InputImpl::toSaurobyteKey(event.key.keysym.scancode),
								event.key.state == SDL_PRESSED,
								event.key.repeat));
					}
					break;

				// Broadcast mouse events
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					{
						MouseButton button = MouseButton::Left;
						if(event.button.button != SDL_BUTTON_LEFT)
							button = event.button.button == SDL_BUTTON_RIGHT ? MouseButton::Right : MouseButton::Middle;

						sendMessage<MouseButtonEvent>(
							event.type == SDL_MOUSEBUTTONDOWN ? "MouseButtonDown" : "MouseButtonUp",
							MouseButtonEvent(
								event.button.x,
								event.button.y,
								event.button.which == SDL_TOUCH_MOUSEID,
								button,
								event.button.state == SDL_PRESSED,
								event.button.clicks));
					}
					break;

				// Shutdown signal
				case SDL_QUIT:
					return false;
			}

		}

		return true;
	}

	void Game::start()
	{
	
		// Engine has been started, so lets show the Window
		getWindow().show();

		while(handleEvents())
		{
			m_frameCounter.update();

			// Process frame start entity cleanup
			m_scenePool.frameCleanup();
			m_entityPool.frameCleanup();

			m_videoDevice->clearBuffers();

			// Process the systems and their entities
			m_systemPool.processSystems();
			
			//glFlush();

			getWindow().swapBuffers();
		}
	}
	void Game::stop()
	{
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}

	void Game::setFps(unsigned int fps)
	{
		m_frameCounter.limitFps(fps);
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

	void Game::sendMessage(const Message &message)
	{
		m_messageCentral.sendMessage(message);
	}
	void Game::sendMessage(const std::string &messageName, Entity *entity)
	{
		m_messageCentral.sendMessage(Message(messageName, entity));
	}

	bool Game::runScript(const std::string &filePath)
	{
		return m_luaEnvironment.runScript(filePath);
	}

	unsigned int Game::getFps() const
	{
		return m_frameCounter.getFps();
	}
	float Game::getDelta() const
	{
		return m_frameCounter.getDelta();
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
	Window& Game::getWindow()
	{
		return m_videoDevice->getWindow();
	}

	LuaEnvironment& Game::getLua()
	{
		return m_luaEnvironment;
	}
	LuaConfig& Game::getConfig()
	{
		return m_luaConfig;
	}

};