#include "ScenePool.hpp"
#include "Game.hpp"
#include "SDL2/SDL.h"

namespace jl
{
	ScenePool::ScenePool(Game *game)
		:
		m_activeScene(nullptr),
		m_game(game)
	{
	}
	ScenePool::~ScenePool()
	{
		frameCleanup();
		for(auto itr = m_scenePool.begin(); itr != m_scenePool.end(); itr++)
			delete itr->second;

		m_scenePool.clear();
	}

	Scene& ScenePool::createScene(const std::string &name)
	{
		auto itr = m_scenePool.find(name);
		if(itr == m_scenePool.end())
		{
			m_scenePool[name] = new Scene(name);
			if(m_activeScene == nullptr)
				changeScene(name);

			return *m_scenePool[name];
		}
		else
			return *itr->second;

	}
	void ScenePool::deleteScene(const std::string &name)
	{
		auto itr = m_scenePool.find(name);
		if(itr != m_scenePool.end())
		{
			m_pendingActions.push_back({itr->second, SceneActions::Delete});
			m_scenePool.erase(itr);
		}
	}

	void ScenePool::detachFromAllScenes(Entity &entity)
	{
		for(auto itr = m_scenePool.begin(); itr != m_scenePool.end(); itr++)
			itr->second->detach(entity);
	}
	void ScenePool::frameCleanup()
	{
		for(std::size_t i = 0; i < m_pendingActions.size(); i++)
		{
			Scene *scene = m_pendingActions[i].scene;
			SceneActions action = m_pendingActions[i].action;

			if(action == SceneActions::Delete)
			{
				// Empty systems if we deleted the active scene
				if(scene == m_activeScene)
					m_game->getSystemPool().emptySystems();

				delete scene;
			}
			else if(action == SceneActions::Change)
			{
				// Clear systems from entities
				m_game->getSystemPool().emptySystems();

				// Refresh entities of new Scene
				for(auto itr = m_activeScene->getEntities().begin(); itr != m_activeScene->getEntities().end(); itr++)
				{
					SDL_Log("Refresh  %i", itr->second->getID());
					itr->second->refresh();
				}
			}
		}

		m_pendingActions.clear();
	}

	void ScenePool::changeScene(const std::string &name)
	{
		auto itr = m_scenePool.find(name);
		if(itr != m_scenePool.end())
		{
			m_pendingActions.push_back({itr->second, SceneActions::Change});
			m_activeScene = itr->second;
			SDL_Log("ACTIVE SCENE %s", name.c_str());
			SDL_Log("SIZE %i", m_activeScene->getEntities().size());
		}
	}
	Scene* ScenePool::getActiveScene()
	{
		return m_activeScene;
	}
	Scene* ScenePool::getScene(const std::string &name)
	{
		auto itr = m_scenePool.find(name);
		if(itr != m_scenePool.end())
			return itr->second;
		else
			return nullptr;
	}
};