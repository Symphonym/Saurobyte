#include "EntityPool.hpp"
#include "Game.hpp"

namespace jl
{

	EntityID EntityPool::m_currentEntityID = 0;

	EntityPool::EntityPool(Game *game)
		:
		m_game(game)
	{

	}
	EntityPool::~EntityPool()
	{
		// Push all pending deletes to the spare pool
		frameCleanup();
		for(std::size_t i = 0; i < m_sparePool.size(); i++)
			delete m_sparePool[i];

		for(auto itr = m_entityPool.begin(); itr != m_entityPool.end(); itr++)
			delete itr->second;

		// Delete entity templates
		for(auto itr = m_entityTemplates.begin(); itr != m_entityTemplates.end(); itr++)
		{
			for(std::size_t i = 0; i < itr->second.size(); i++)
				delete itr->second[i];
		}

		m_sparePool.clear();
		m_entityPool.clear();
		m_entityTemplates.clear();
		m_currentEntityID = 0;
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
			newEntity = new Entity(m_currentEntityID++, m_game);

		m_entityPool[newEntity->getID()] = newEntity;
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
		auto itr = m_entityPool.find(entity.getID());
		if(itr != m_entityPool.end())
		{
			m_pendingActions.push_back({&entity, EntityActions::Kill});
			m_entityPool.erase(itr);
		}
	}
	void EntityPool::refreshEntity(Entity &entity)
	{
		m_pendingActions.push_back({&entity, EntityActions::Refresh});
	}
	void EntityPool::detachEntity(Entity &entity)
	{
		m_pendingActions.push_back({&entity, EntityActions::Detach});
	}
	void EntityPool::saveEntity(const std::string &templateName, Entity &entity)
	{
		// Overwrite existing save
		auto findItr = m_entityTemplates.find(templateName);
		if(findItr != m_entityTemplates.end())
		{
			for(std::size_t i = 0; i < findItr->second.size(); i++)
				delete findItr->second[i];

			findItr->second.clear();
		}

		std::vector<BaseComponent*>& componentSetup = m_entityTemplates[templateName];
		for(auto itr = entity.getComponents().begin(); itr != entity.getComponents().end(); itr++)
			componentSetup.push_back(itr->second->clone());
	}

	void EntityPool::frameCleanup()
	{
		for(std::size_t i = 0; i < m_pendingActions.size(); i++)
		{
			Entity *entity = m_pendingActions[i].entity;
			EntityActions action = m_pendingActions[i].action;

			if(action == EntityActions::Kill)
			{
				// Detach entity from other systems
				m_game->getSystemPool().removeEntityFromSystems(*entity);
				m_game->getScenePool().detachFromAllScenes(*entity);

				// Strip entity of components, deactivate it and push
				// it into spare pool
				entity->removeAllComponents();
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
				// Detach entity from other systems
				m_game->getSystemPool().removeEntityFromSystems(*entity);
			}

		}
		m_pendingActions.clear();
	}

	Entity* EntityPool::getEntity(EntityID id)
	{
		auto itr = m_entityPool.find(id);
		return itr != m_entityPool.end() ? itr->second : nullptr;
	}
	std::size_t EntityPool::getEntityCount() const
	{
		return m_entityPool.size();
	}
};