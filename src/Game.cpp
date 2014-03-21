#include "Game.hpp"

namespace jl
{
	Game::Game()
		:
		m_entityPool(this),
		m_systemPool(this),
		m_scenePool(this),
		m_messageCentral()
	{

	}
	Game::~Game()
	{

	}

	void Game::updateGame()
	{
		m_scenePool.frameCleanup();
		m_entityPool.frameCleanup();

		m_systemPool.processSystems();
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
};