#ifndef JL_SYSTEM_HPP
#define JL_SYSTEM_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/MessageHandler.hpp>

namespace Saurobyte
{

	class Engine;
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

		Engine *engine;
		
		BaseSystem(Engine *engine, TypeID typeID);

	public:

		virtual ~BaseSystem() {};

		/**
		 * Adds a combination of component types that this system will process
		 * @param componentIDs Vector of the component ID's in this component combination
		 */
		void addRequirement(const std::vector<TypeID> &componentIDs);

		/**
		 * Removes an entity from the system, removing it from the processing cycle of the system
		 * @param entity    The entity to remove
		 * @param wasKilled Whether or not the cause of removal was entity termination
		 */
		void removeEntity(Entity &entity, bool wasKilled);

		/**
		 * Updates the status of an entity within the system, making sure it has the sought after components
		 * @param entity The entity to update
		 */
		void refreshEntity(Entity &entity);

		/**
		 * Sets the active status of the system, inactive systems will not run the processing cycle
		 * @param active Boolean for whether or not the system should be active
		 */
		void setActive(bool active);

		/**
		 * Called before processEntity is called
		 */
		virtual void preProcess() {};
		/**
		 * Called each frame on each entity that is processed by this system
		 * @param entity The entity that the system is currently processing
		 */
		virtual void processEntity(Entity &entity) = 0;
		/**
		 * Called after processEntity is called
		 */
		virtual void postProcess() {};


		/**
		 * Called when an entity is added for processing in the system
		 * @param entity The added entity
		 */
		virtual void onAttach(Entity &entity) {};
		/**
		 * Called when an entity is removed from further processing in the system
		 * @param entity The removed entity
		 */
		virtual void onDetach(Entity &entity) {};
		/**
		 * Called when the system is cleared from all of its entities (e.g switching scenes), allowing for cleanup
		 */
		virtual void onClear() {};
		/**
		 * Called before onDetach if the removal reason was entity termination
		 * @param entity The entity that was killed
		 */
		virtual void onKill(Entity &entity) {};

		/**
		 * Clear the system of all its entities
		 */
		void clearSystem();

		/**
		 * Get the unique type identifier of this system, indicating what type of system it is
		 * @return The unique identifier
		 */
		TypeID getTypeID() const;
		/**
		 * Returns whether or not the system is active
		 * @return Active status
		 */
		bool isActive() const;

		/**
		 * Returns a read-only map of all the entities in the system
		 */
		const std::unordered_map<EntityID, Entity*>& getEntities() const;

	};

	template<typename TType> class System : public BaseSystem
	{
	public:
		
		System(Engine *engine)
			:
			BaseSystem(engine, TypeIdGrabber::getUniqueTypeID<TType>())
		{}
		virtual ~System() {};
	};
};

#endif