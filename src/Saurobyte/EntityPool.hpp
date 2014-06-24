/*

	The MIT License (MIT)

	Copyright (c) 2014 by Jakob Larsson

	Permission is hereby granted, free of charge, to any person obtaining 
	a copy of this software and associated documentation files (the "Software"), 
	to deal in the Software without restriction, including without limitation the 
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
	sell copies of the Software, and to permit persons to whom the Software is 
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in 
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
	IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

#ifndef SAUROBYTE_ENTITYIDPOOL_HPP
#define SAUROBYTE_ENTITYIDPOOL_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <Saurobyte/IdentifierTypes.hpp>
#include <Saurobyte/Entity.hpp>

namespace Saurobyte
{
	class Engine;
	class EntityPool
	{	
	private:

		typedef std::unique_ptr<Entity> EntityPtr;

		std::vector<EntityPtr> m_entityPool;
		std::unordered_map<std::string, std::vector<ComponentPtr> > m_entityTemplates;

		// Pointers to entities that are available for reuse
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

		Engine *const m_engine;

	public:

		EntityPool(Engine *engine);
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