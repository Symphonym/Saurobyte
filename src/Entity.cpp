#include "Entity.hpp"
#include "EntityPool.hpp"
#include "Game.hpp"

namespace jl
{
	Entity::Entity(EntityID id, Game *game)
		:
		m_isActive(true),
		m_id(id),
		m_game(game)
	{

	}
	Entity::~Entity()
	{
		// Immidiately delete all components
		for(auto iter = m_components.begin(); iter != m_components.end(); iter++)
			delete iter->second;
		m_components.clear();
		m_luaComponents.clear();
	}

	void Entity::addComponent(TypeID id, BaseComponent *component)
	{
		// Make sure the component doesn't exist, then add it
		auto iter = m_components.find(component->getTypeID());

		// Overwrite existing component, make sure it's not the same
		if(iter != m_components.end() && component != iter->second)
			delete iter->second;

		m_components[component->getTypeID()] = component;

		// Save component by name as well if it has one, used by Lua
		if(!component->getName().empty())
			m_luaComponents[component->getName()] = component;

		m_game->getEntityPool().refreshEntity(*this);
	}
	void Entity::removeComponent(TypeID id)
	{
		auto iter = m_components.find(id);

		if(iter != m_components.end())
		{

			// Remove from name map as well
			auto nameItr = m_luaComponents.find(iter->second->getName());
			if(nameItr != m_luaComponents.end())
				m_luaComponents.erase(nameItr);

			delete iter->second;
			m_components.erase(iter);
			m_game->getEntityPool().refreshEntity(*this);
		}

	}
	BaseComponent* const Entity::getComponent(TypeID id)
	{
		auto iter = m_components.find(id);

		return iter == m_components.end() ? nullptr : iter->second;
	}
	BaseComponent* const Entity::getComponent(const std::string &componentName)
	{
		auto iter = m_luaComponents.find(componentName);

		return iter == m_luaComponents.end() ? nullptr : iter->second;
	}
	bool Entity::hasComponent(TypeID id)
	{
		auto iter = m_components.find(id);
		return iter != m_components.end();
	}

	void Entity::removeAllComponents()
	{
		// Don't run removeComponent function so we can refresh the entity
		// only once instead.
		for(auto iter = m_components.begin(); iter != m_components.end(); iter++)
			delete iter->second;

		m_components.clear();
		m_luaComponents.clear();

		m_game->getEntityPool().refreshEntity(*this);
	}

	void Entity::refresh()
	{
		m_game->getEntityPool().refreshEntity(*this);
	}
	void Entity::kill()
	{
		m_game->getEntityPool().killEntity(*this);
	}
	void Entity::detach()
	{
		m_game->getEntityPool().detachEntity(*this);
	}
	void Entity::save(const std::string &templateName)
	{
		m_game->getEntityPool().saveEntity(templateName, *this);
	}
	void Entity::setActive(bool active)
	{
		m_isActive = active;
	}

	void Entity::cloneFrom(Entity &entity)
	{
		// Clone the entity copy's components
		for(auto iter = entity.m_components.begin(); iter != entity.m_components.end(); iter++)
			this->addComponent(iter->second->getTypeID(), iter->second->clone());

		m_game->getEntityPool().refreshEntity(*this);
	}

	std::size_t Entity::getComponentCount() const
	{
		return m_components.size();
	}
	ComponentBag& Entity::getComponents()
	{
		return m_components;
	}
	EntityID Entity::getID() const
	{
		return m_id;
	}
	bool Entity::isActive() const
	{
		return m_isActive;
	}

};