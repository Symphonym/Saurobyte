#ifndef JL_ENTITY_HPP
#define JL_ENTITY_HPP

#include <unordered_map>
#include <vector>
#include "IdentifierTypes.hpp"
#include "Component.hpp"

namespace jl
{

	typedef std::unordered_map<TypeID, BaseComponent*> ComponentBag;
	typedef std::unordered_map<std::string, BaseComponent*> LuaComponentBag;

	class Game;
	class Entity
	{
	private:

		// Entities own their components
		ComponentBag m_components;
		LuaComponentBag m_luaComponents;

		bool m_isActive;

		EntityID m_id;
		Game *m_game;

	public:

		explicit Entity(EntityID id, Game *game);
		~Entity();
		
		// Entities may not be copied, their components can be cloned though
		Entity(const Entity &rhs) = delete;
		Entity& operator= (const Entity &rhs) = delete;

		// Sends a request to have the specified component added, it will be processed
		// at the start of the next frame.
		template<typename TType, typename ...TArgs> void addComponent(TArgs&&... args)
		{
			addComponent(TypeIdGrabber::getUniqueTypeID<TType>(), new TType(args...));
		};
		void addComponent(TypeID id, BaseComponent *component);

		// Sends a request to have the specified component removed, it will be processed
		// at the start of the next frame.
		template<typename TType> void removeComponent()
		{
			removeComponent(TypeIdGrabber::getUniqueTypeID<TType>());
		};
		void removeComponent(TypeID id);

		// Attempts to retrieve the specified component from the entity, returns 'nullptr'
		// if the component wasen't found. The return value is read-write enabled.
		template<typename TType> TType* const getComponent()
		{
			BaseComponent *comp = getComponent(TypeIdGrabber::getUniqueTypeID<TType>());

			return comp == nullptr ? nullptr : static_cast<TType*>(comp);
		};
		BaseComponent* const getComponent(TypeID id);

		// Used by Lua
		BaseComponent* const getComponent(const std::string &componentName);

		// Returns whether or not the specified component exists within the entity
		template<typename TType> bool hasComponent()
		{
			return hasComponent(TypeIdGrabber::getUniqueTypeID<TType>());
		}
		bool hasComponent(TypeID id);
		void removeAllComponents();

		void refresh();
		void kill();
		void detach();
		void save(const std::string &templateName);
		void setActive(bool active);

		// Clones the components the target entity into this entity. Existing
		// components that conflicts will be overwritten, others will remain
		// untouched.
		void cloneFrom(Entity &entity);

		std::size_t getComponentCount() const;

		// Returns a read-write enabled map of all the internal components
		ComponentBag& getComponents();

		EntityID getID() const;
		// Whether or not the Entity is active, inactive entities do not
		// get processed at all.
		bool isActive() const;
	};
};

#endif