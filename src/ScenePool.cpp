#include "ScenePool.hpp"
#include "Game.hpp"
#include "Message.hpp"
#include "Logger.hpp"

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
			JL_DEBUG_LOG("Created new scene '%s'", name.c_str());
			
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

				JL_DEBUG_LOG("Deleting scene '%s'", scene->getName().c_str());
				delete scene;
			}
			else if(action == SceneActions::Change)
			{
				// Clear systems from entities
				m_game->getSystemPool().emptySystems();

				JL_DEBUG_LOG("Changing to scene '%s'", scene->getName().c_str());

				// Refresh entities of new Scene, instantly
				for(auto itr = scene->getEntities().begin(); itr != scene->getEntities().end(); itr++)
					itr->second->refresh();

				m_game->queueMessage(createMessage<std::string>("SceneLoad", scene->getName()));
			}
		}

		m_pendingActions.clear();
	}

	void ScenePool::changeScene(const std::string &name)
	{
		// Scene changes must be done at start of frame because otherwise Systems
		// might clear their own entity map while iterating it: Not Good.
		auto itr = m_scenePool.find(name);
		if(itr != m_scenePool.end())
		{
			m_activeScene = itr->second;
			m_pendingActions.push_back({itr->second, SceneActions::Change});
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