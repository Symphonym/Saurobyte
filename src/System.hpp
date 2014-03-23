#ifndef JL_SYSTEM_HPP
#define JL_SYSTEM_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include "IdentifierTypes.hpp"
#include "MessageHandler.hpp"

namespace jl
{

	class Game;
	class Entity;
	class BaseSystem : public MessageHandler
	{
	private:

		friend class SystemPool;

		// List of component combinations this system wants
		std::vector< std::vector<TypeID> > m_wantedEntities;
		std::unordered_map<EntityID, Entity*> m_monitoredEntities;

		const TypeID m_systemType;
		bool m_isActive;

		void processEntities();

	protected:

		Game *game;
		
		BaseSystem(Game *game, TypeID typeID);

	public:

		virtual ~BaseSystem() {};

		// Map an entity component requirement to the specified name for easier future access.
		void addRequirement(const std::vector<TypeID> &componentIDs);

		// Remove an entity from the System, making the System no longer
		// monitor it
		void removeEntity(Entity &entity);

		// Updates an already monitored entity if components have changed to
		// whether or not the requirements still hold
		void refreshEntity(Entity &entity);

		void setActive(bool active);

		// pre and postProcess allows for setup/cleanup before or
		// after entities are processed by the system.
		virtual void preProcess() {};
		virtual void processEntity(Entity &entity) = 0;
		virtual void postProcess() {};

		// Called when entities are added/removed to the system, allowing for
		// setup or cleanup
		virtual void onEntityAdded(Entity &entity) {};
		virtual void onEntityRemoved(Entity &entity) {};
		// Called when a system is completely cleared of Entities, used mostly for
		// cleanup
		virtual void onSystemCleared() {};

		void clearSystem();

		TypeID getTypeID() const;
		bool isActive() const;
		const std::unordered_map<EntityID, Entity*>& getEntities() const;

	};

	template<typename TType> class System : public BaseSystem
	{
	public:
		
		System(Game *game)
			:
			BaseSystem(game, TypeIdGrabber::getUniqueTypeID<TType>())
		{}
		virtual ~System() {};
	};
};

#endif