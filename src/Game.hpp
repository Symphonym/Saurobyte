#ifndef JL_GAME_HPP
#define JL_GAME_HPP

#include "EntityPool.hpp"
#include "SystemPool.hpp"
#include "ScenePool.hpp"
#include "MessageCentral.hpp"
#include "LuaEnvironment.hpp"
#include "OpenGLWindow.hpp"

namespace jl
{
	enum GameLogging
	{
		Debug, // Enables ALL logging
		Warning_Error, // Enables logging for WARNING and ERROR messages
		Info_Error, // Enables logging for INFO and ERROR messages 		 -- DEFAULT
		None // Disables ALL logging
	};

	class Game
	{
	private:

		EntityPool m_entityPool;
		SystemPool m_systemPool;
		ScenePool m_scenePool;
		MessageCentral m_messageCentral;
		LuaEnvironment m_luaEnvironment;

		OpenGLWindow &m_glWindow;

	public:


		Game(OpenGLWindow &window, GameLogging logging = GameLogging::Info_Error);
		~Game();

		void gameLoop();

		// Creating entities and scenes
		Entity& createEntity();
		Entity& createEntity(const std::string &templateName);
		Scene& createScene(const std::string &name);

		// Adding systems
		template<typename TType, typename ...TArgs> void addSystem(TArgs&... args)
		{
			m_systemPool.addSystem<TType>(args...);
		};

		// Scene managing
		Scene* getActiveScene();
		void changeScene(const std::string &sceneName);

		// Message broadcasting
		void broadcast(Message *message);

		// Running Lua scripts
		bool runScript(const std::string &filePath);

		EntityPool& getEntityPool();
		SystemPool& getSystemPool();
		ScenePool& getScenePool();
		MessageCentral& getMessageCentral();
		LuaEnvironment& getLua();
		OpenGLWindow& getWindow();
	};
};

#endif