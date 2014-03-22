#include "Game.hpp"
#include "Message.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace jl
{
	Game::Game(OpenGLWindow &window)
		:
		m_entityPool(this),
		m_systemPool(this),
		m_scenePool(this),
		m_messageCentral(),
		m_glWindow(window)
	{

	}
	Game::~Game()
	{

	}

	void Game::gameLoop()
	{

		SDL_Event event;

		while(m_glWindow.running())
		{
			// Process frame start entity cleanup
			m_scenePool.frameCleanup();
			m_entityPool.frameCleanup();


			// Poll events
			while(m_glWindow.pollEvent(event))
			{
				if(event.type == SDL_QUIT)
					m_glWindow.close();
				else if(event.type == SDL_KEYDOWN)
				{
					broadcast(createMessage<SDL_Event>("KeyDown", event));

					if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
						m_glWindow.close();
					else if(event.key.keysym.scancode == SDL_SCANCODE_W)
						broadcast(createMessage("ReloadLua"));

				}
				else if(event.type == SDL_KEYUP)
					broadcast(createMessage<SDL_Event>("KeyUp", event));
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

			m_glWindow.swapBuffers();
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

	void Game::broadcast(Message *message)
	{
		m_messageCentral.broadcast(message);
	}

	void Game::killEntity(Entity& entity)
	{
		m_entityPool.killEntity(entity);
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
	OpenGLWindow& Game::getWindow()
	{
		return m_glWindow;
	}
};