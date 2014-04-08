#include "Entity.hpp"
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
		m_components.clear();
		m_luaComponents.clear();
	}

	void Entity::addComponent(TypeID id, BaseComponent *component)
	{
		// Make sure the component doesn't exist, then add it
		auto itr = m_components.find(component->typeID);

		// Overwrite existing component, make sure it's not the same
		if(itr != m_components.end() && component != itr->second.get())
			m_components.erase(itr);

		m_components[component->typeID] = ComponentPtr(component);

		// Save component by name as well, used by Lua
		m_luaComponents[component->getName()] = component;

		refresh();
	}
	void Entity::removeComponent(TypeID id)
	{
		auto itr = m_components.find(id);

		// Remove component if it exists
		if(itr != m_components.end())
		{

			// Remove from name map as well
			auto nameItr = m_luaComponents.find(itr->second->getName());
			if(nameItr != m_luaComponents.end())
				m_luaComponents.erase(nameItr);

			m_components.erase(itr);
			refresh();
		}

	}
	BaseComponent* const Entity::getComponent(TypeID id)
	{
		auto iter = m_components.find(id);

		return iter == m_components.end() ? nullptr : iter->second.get();
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
		m_components.clear();
		m_luaComponents.clear();

		refresh();
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
			this->addComponent(iter->second->typeID, iter->second->clone());

		refresh();
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