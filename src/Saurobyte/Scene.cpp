#include <Saurobyte/Scene.hpp>
#include <Saurobyte/Entity.hpp>

namespace Saurobyte
{
	Scene::Scene(const std::string &name)
		:
		m_sceneName(name)
	{

	}

	void Scene::attach(Entity &entity)
	{
		if(entity.m_scene != nullptr)
			entity.m_scene->detach(entity);

		entity.m_scene = this;

		m_entities[entity.getID()] = &entity;
		entity.refresh();
	}
	void Scene::detach(Entity &entity)
	{
		auto itr = m_entities.find(entity.getID());
		if(itr != m_entities.end())
		{
			entity.detach();
			entity.m_scene = nullptr;
			m_entities.erase(itr);
		}
	}

	bool Scene::contains(Entity &entity)
	{
		return m_entities.find(entity.getID()) != m_entities.end();
	}

	const std::unordered_map<EntityID, Entity*>& Scene::getEntities()
	{
		return m_entities;
	}
	std::string Scene::getName() const
	{
		return m_sceneName;
	}
	Camera& Scene::getCamera()
	{
		return m_sceneCamera;
	}
};