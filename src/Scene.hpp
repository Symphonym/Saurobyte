#ifndef JL_SCENE_HPP
#define JL_SCENE_HPP

#include <string>
#include <unordered_map>
#include "IdentifierTypes.hpp"

namespace jl
{
	class Entity;
	class Scene
	{
	private:

		std::string m_sceneName;
		std::unordered_map<EntityID, Entity*> m_entities;

	public:

		Scene(const std::string &name);

		void attach(Entity &entity);
		void detach(Entity &entity);

		bool contains(Entity &entity);

		const std::unordered_map<EntityID, Entity*>& getEntities();
		std::string getName() const;
	};
};

#endif