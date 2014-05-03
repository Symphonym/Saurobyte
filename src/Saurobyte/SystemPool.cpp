#include <Saurobyte/SystemPool.hpp>
#include <Saurobyte/System.hpp>


namespace jl
{
	SystemPool::SystemPool(Game *game)
		:
		m_game(game)
	{
	}
	SystemPool::~SystemPool()
	{
		frameCleanup();

		m_systemPool.clear();
	}

	void SystemPool::addSystem(BaseSystem *newSystem)
	{
		// Make sure the system doesn't exist, then add it
		auto iter = m_systemPool.find(newSystem->getTypeID());
		if(iter == m_systemPool.end())
			m_systemPool[newSystem->getTypeID()] = SystemPtr(newSystem);
	}
	void SystemPool::removeSystem(TypeID id)
	{
		auto iter = m_systemPool.find(id);

		// Delete the system from the map, but the actual memory deletion is done at the start
		// of the next frame. This is done so any calls to hasSystem after the remove call will
		// return false.
		if(iter != m_systemPool.end())
		{
			m_pendingDeletes.push_back(std::move(iter->second));
			m_systemPool.erase(iter);
		}
	}
	BaseSystem* SystemPool::getSystem(TypeID id)
	{
		auto iter = m_systemPool.find(id);

		if(iter == m_systemPool.end())
			return nullptr;
		else
			return iter->second.get();
	}
	bool SystemPool::hasSystem(TypeID id)
	{
		auto iter = m_systemPool.find(id);

		return iter != m_systemPool.end();
	}

	void SystemPool::emptySystems()
	{
		for(auto itr = m_systemPool.begin(); itr != m_systemPool.end(); itr++)
		{
			if(itr->second->isActive())
				itr->second->clearSystem();
		}
	}
	void SystemPool::processSystems()
	{
		for(auto itr = m_systemPool.begin(); itr != m_systemPool.end(); itr++)
		{
			if(itr->second->isActive())
			{
				itr->second->preProcess();
				itr->second->processEntities();
				itr->second->postProcess();
			}
		}
	}

	void SystemPool::removeEntityFromSystems(Entity &entity, bool wasKilled)
	{
		for(auto itr = m_systemPool.begin(); itr != m_systemPool.end(); itr++)
			itr->second->removeEntity(entity, wasKilled);
	}
	void SystemPool::refreshEntity(Entity &entity)
	{
		for(auto itr = m_systemPool.begin(); itr != m_systemPool.end(); itr++)
			itr->second->refreshEntity(entity);
	}

	void SystemPool::frameCleanup()
	{
		m_pendingDeletes.clear();
	}

};