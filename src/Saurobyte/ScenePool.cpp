#include <Saurobyte/ScenePool.hpp>
#include <Saurobyte/Game.hpp>
#include <Saurobyte/Message.hpp>
#include <Saurobyte/Logger.hpp>

namespace Saurobyte
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

		m_scenePool.clear();
	}

	Scene& ScenePool::createScene(const std::string &name)
	{
		auto itr = m_scenePool.find(name);
		if(itr == m_scenePool.end())
		{
			m_scenePool[name] = ScenePtr(new Scene(name));
			SAUROBYTE_DEBUG_LOG("Created new scene '", name, "'");
			
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
			m_pendingActions.push_back({itr->second.release(), SceneActions::Delete});
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

				SAUROBYTE_DEBUG_LOG("Deleting scene '", scene->getName(), "'");
				delete scene;
			}
			else if(action == SceneActions::Change)
			{
				// Clear systems from entities
				m_game->getSystemPool().emptySystems();

				SAUROBYTE_DEBUG_LOG("Changing to scene '", scene->getName(), "'");

				// Refresh entities of new Scene, instantly
				for(auto itr = scene->getEntities().begin(); itr != scene->getEntities().end(); itr++)
					itr->second->refresh();

				// TODOm_game->queueMessage(MessageData<std::string>("SceneLoad", scene->getName()));
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
			m_activeScene = itr->second.get();
			m_pendingActions.push_back({itr->second.get(), SceneActions::Change});
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
			return itr->second.get();
		else
			return nullptr;
	}
};