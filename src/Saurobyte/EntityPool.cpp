#include <Saurobyte/EntityPool.hpp>
#include <Saurobyte/Game.hpp>

namespace jl
{

	EntityPool::EntityPool(Game *game)
		:
		m_game(game)
	{

	}
	EntityPool::~EntityPool()
	{
		// Push all pending deletes to the spare pool
		frameCleanup();

		m_sparePool.clear();
		m_entityPool.clear();
		m_entityTemplates.clear();
	}

	Entity& EntityPool::createEntity()
	{
		Entity *newEntity = nullptr;
		if(m_sparePool.size() > 0)
		{
			newEntity = m_sparePool.back();
			newEntity->setActive(true);
			m_sparePool.pop_back();
		}
		else
			newEntity = new Entity(m_entityPool.size(), m_game);

		m_entityPool.push_back(EntityPtr(newEntity));
		return *newEntity;
	}
	Entity& EntityPool::createEntity(const std::string &templateName)
	{
		Entity& ent = createEntity();

		// Clone components from template
		auto itr = m_entityTemplates.find(templateName);
		if(itr != m_entityTemplates.end())
		{
			for(std::size_t i = 0; i < itr->second.size(); i++)
				ent.addComponent(itr->second[i]->getTypeID(), itr->second[i]->clone());
		}

		return ent;
	}
	void EntityPool::killEntity(Entity &entity)
	{
		// Push kill request
		m_pendingActions.push_back({&entity, EntityActions::Kill});
	}
	void EntityPool::refreshEntity(Entity &entity)
	{
		// Push refresh request
		m_pendingActions.push_back({&entity, EntityActions::Refresh});
	}
	void EntityPool::detachEntity(Entity &entity)
	{
		// Push detach request
		m_pendingActions.push_back({&entity, EntityActions::Detach});
	}
	void EntityPool::saveEntity(const std::string &templateName, Entity &entity)
	{
		// Overwrite existing save
		auto findItr = m_entityTemplates.find(templateName);
		if(findItr != m_entityTemplates.end())
			findItr->second.clear();

		std::vector<ComponentPtr>& componentSetup = m_entityTemplates[templateName];
		for(auto itr = entity.getComponents().begin(); itr != entity.getComponents().end(); itr++)
			componentSetup.push_back(ComponentPtr(itr->second->clone()));
	}

	void EntityPool::frameCleanup()
	{
		for(std::size_t i = 0; i < m_pendingActions.size(); i++)
		{
			Entity *entity = m_pendingActions[i].entity;
			EntityActions action = m_pendingActions[i].action;

			if(action == EntityActions::Kill)
			{
					// Detach entity from other systems first since we might
					// want to do use data from the components just before they are removed.
					m_game->getSystemPool().removeEntityFromSystems(*entity, true);
					m_game->getScenePool().detachFromAllScenes(*entity);

					// Strip entity of components, deactivate it and push
					// it into spare pool
					entity->removeAllComponents(); // This call refreshes the entity as well
					entity->setActive(false);


					m_sparePool.push_back(entity);
			}
			else if(action == EntityActions::Refresh)
			{
				Scene* activeScene = m_game->getScenePool().getActiveScene();

				// Refresh entity monitoring status if it's in the active scene
				if(activeScene != nullptr && activeScene->contains(*entity))
					m_game->getSystemPool().refreshEntity(*entity);
			}
			else if(action == EntityActions::Detach)
			{
				// Detach entity from all systems
				m_game->getSystemPool().removeEntityFromSystems(*entity, false);
			}


		}
		m_pendingActions.clear();
	}

	Entity& EntityPool::getEntity(EntityID id)
	{
		return *m_entityPool.at(id);
	}
	std::size_t EntityPool::getEntityCount() const
	{
		return m_entityPool.size();
	}
};