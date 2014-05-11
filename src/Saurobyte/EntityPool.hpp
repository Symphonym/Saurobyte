#ifndef JL_ENTITYIDPOOL_HPP
#define JL_ENTITYIDPOOL_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/Entity.hpp>

namespace Saurobyte
{
	class Game;
	class EntityPool
	{	
	private:

		typedef std::unique_ptr<Entity> EntityPtr;

		std::vector<EntityPtr> m_entityPool;
		std::unordered_map<std::string, std::vector<ComponentPtr> > m_entityTemplates;

		// Killed entities, that will be recycled whenever a new entity is wanted
		std::vector<Entity*> m_sparePool;

		enum EntityActions
		{
			Kill, // Kills the entity, stripping it of components and making it reusable
			Refresh, // Refreshes the monitoring status of the entity within the systems
			Detach // Removes entity from all systems
		};
		struct EntityAction
		{
			Entity *entity;
			EntityActions action;
		};
		std::vector<EntityAction> m_pendingActions;

		Game *const m_game;

	public:

		EntityPool(Game *game);
		~EntityPool();

		// Create empty entity or create by template
		Entity& createEntity();
		Entity& createEntity(const std::string &templateName);

		// Kills the entity, stripping it of components and recycling it
		void killEntity(Entity &entity);
		// Refreshes the entity, updating its monitoring status with systems
		void refreshEntity(Entity &entity);
		// Detaches the entity from all systems
		void detachEntity(Entity &entity);
		// Saves the component setup of an entity and stores it by a string name
		void saveEntity(const std::string &templateName, Entity &entity);

		// Retrieves an entity by id, returns null if it could not be found
		Entity& getEntity(EntityID id);
		std::size_t getEntityCount() const;

		void frameCleanup();

	};
};

#endif