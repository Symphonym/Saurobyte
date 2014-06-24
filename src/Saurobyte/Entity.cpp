#include <Saurobyte/Entity.hpp>
#include <Saurobyte/Engine.hpp>

namespace Saurobyte
{
	Entity::Entity(EntityID id, Engine *engine)
		:
		m_isActive(true),
		m_id(id),
		m_engine(engine),
		m_scene(nullptr)
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
		auto itr = m_components.find(component->getTypeID());

		// Due to the nature of unique_ptr, existing components will be
		// overwrited, so make sure we're not adding the same component twice.
		if(itr != m_components.end() && component == itr->second.get())
			return;

		m_components[component->getTypeID()] = ComponentPtr(component);

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
		m_engine->getEntityPool().refreshEntity(*this);
	}
	void Entity::kill()
	{
		m_engine->getEntityPool().killEntity(*this);
	}
	void Entity::detach()
	{
		m_engine->getEntityPool().detachEntity(*this);
	}
	void Entity::save(const std::string &templateName)
	{
		m_engine->getEntityPool().saveEntity(templateName, *this);
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
	bool Entity::inScene() const
	{
		return m_scene != nullptr;
	}

};