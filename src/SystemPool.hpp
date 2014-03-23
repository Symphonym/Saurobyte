#ifndef JL_SYSTEMPOOL_HPP
#define JL_SYSTEMPOOL_HPP

#include <unordered_map>
#include <vector>
#include "IdentifierTypes.hpp"
#include "System.hpp"

namespace jl
{
	class Game;
	class Entity;
	class SystemPool
	{
	private:

		std::unordered_map<TypeID, BaseSystem*> m_systemPool;
		std::vector<BaseSystem*> m_pendingDeletes;

		Game *m_game;

	public:

		SystemPool(Game *game);
		~SystemPool();

		template<typename TType, typename ...TArgs> TType* addSystem(TArgs&... args)
		{
			addSystem(TypeIdGrabber::getUniqueTypeID<TType>(), new TType(m_game, args...));
		};
		void addSystem(TypeID id, BaseSystem *newSystem);


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

		void removeEntityFromSystems(Entity &entity);
		void refreshEntity(Entity &entity);

		void emptySystems();
		void processSystems();
		void frameCleanup();
	};
};

#endif