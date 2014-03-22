#ifndef JL_GAME_HPP
#define JL_GAME_HPP

#include "EntityPool.hpp"
#include "SystemPool.hpp"
#include "ScenePool.hpp"
#include "MessageCentral.hpp"
#include "OpenGLWindow.hpp"

namespace jl
{
	class Game
	{
	private:

		EntityPool m_entityPool;
		SystemPool m_systemPool;
		ScenePool m_scenePool;
		MessageCentral m_messageCentral;

		OpenGLWindow &m_glWindow;

	public:


		Game(OpenGLWindow &window);
		~Game();

		void gameLoop();

		// Creating entities and scenes
		Entity& createEntity();
		Entity& createEntity(const std::string &templateName);
		Scene& createScene(const std::string &name);

		// Adding systems
		template<typename TType, typename ...TArgs> void addSystem(TArgs&... args)
		{
			m_systemPool.addSystem<TType>(args...);
		};

		// Scene managing
		Scene* getActiveScene();
		void changeScene(const std::string &sceneName);

		// Message broadcasting
		void broadcast(Message *message);

		void killEntity(Entity& entity);

		EntityPool& getEntityPool();
		SystemPool& getSystemPool();
		ScenePool& getScenePool();
		MessageCentral& getMessageCentral();
		OpenGLWindow& getWindow();
	};
};

#endif