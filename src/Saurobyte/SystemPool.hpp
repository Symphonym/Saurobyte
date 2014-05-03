#ifndef JL_SYSTEMPOOL_HPP
#define JL_SYSTEMPOOL_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/System.hpp>

namespace jl
{
	class Game;
	class Entity;
	class SystemPool
	{
	private:

		typedef std::unique_ptr<BaseSystem> SystemPtr;

		std::unordered_map<TypeID, SystemPtr> m_systemPool;
		std::vector<SystemPtr> m_pendingDeletes;

		Game *m_game;

	public:

		SystemPool(Game *game);
		~SystemPool();

		void addSystem(BaseSystem *newSystem);


		template<typename TType> void removeSystem()
		{
			removeSystem(TypeIdGrabber::getUniqueTypeID<TType>());
		};
		void removeSystem(TypeID id);

		// Attempts to retrieve the specified component from the entity, returns 'nullptr'
		// if the component wasen't found. The return value is read-write enabled.
		template<typename TType> TType* getSystem()
		{
			BaseSystem *sys = getSystem(TypeIdGrabber::getUniqueTypeID<TType>());

			return sys == nullptr ? nullptr : static_cast<TType*>(sys);
		};
		BaseSystem* getSystem(TypeID id);

		// Returns whether or not the specified component exists within the entity
		template<typename TType> bool hasSystem()
		{
			return hasComponent(TypeIdGrabber::getUniqueTypeID<TType>());
		}
		bool hasSystem(TypeID id);

		void removeEntityFromSystems(Entity &entity, bool wasKilled = false);
		void refreshEntity(Entity &entity);

		void emptySystems();
		void processSystems();
		void frameCleanup();
	};
};

#endif