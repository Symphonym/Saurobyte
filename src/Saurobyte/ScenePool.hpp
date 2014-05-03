#ifndef JL_SCENEPOOL_HPP
#define JL_SCENEPOOL_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <Saurobyte/Scene.hpp>

namespace jl
{
	class Game;
	class ScenePool
	{
	private:

		typedef std::unique_ptr<Scene> ScenePtr;

		std::unordered_map<std::string, ScenePtr> m_scenePool;
		Scene *m_activeScene;

		enum SceneActions
		{
			Delete, // Deletes the scene
			Change // Changes the scene
		};
		struct SceneAction
		{
			Scene *scene;
			SceneActions action;
		};
		std::vector<SceneAction> m_pendingActions;

		Game *m_game;

	public:

		ScenePool(Game *game);
		~ScenePool();

		Scene& createScene(const std::string &name);
		void deleteScene(const std::string &name);

		void frameCleanup();

		void detachFromAllScenes(Entity &entity);

		void changeScene(const std::string &name);
		Scene* getActiveScene();
		Scene* getScene(const std::string &name);

	};
};

#endif