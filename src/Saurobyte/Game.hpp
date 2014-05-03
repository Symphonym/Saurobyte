#ifndef JL_GAME_HPP
#define JL_GAME_HPP

#include <Saurobyte/EntityPool.hpp>
#include <Saurobyte/SystemPool.hpp>
#include <Saurobyte/ScenePool.hpp>
#include <Saurobyte/MessageCentral.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/OpenGLWindow.hpp>
#include <Saurobyte/AudioDevice.hpp>

namespace jl
{
	enum class GameLogging
	{
		Debug, // Enables ALL logging
		Warning_Error, // Enables logging for WARNING and ERROR messages
		Info_Error, // Enables logging for INFO and ERROR messages 		 -- DEFAULT
		None // Disables ALL logging
	};

	enum class OpenGLVersions
	{
		Core_3_3,
		Core_4_3,
		ES_3_3,
		ES_4_3
	};

	class Game
	{
	private:

		EntityPool m_entityPool;
		SystemPool m_systemPool;
		ScenePool m_scenePool;
		MessageCentral m_messageCentral;
		LuaEnvironment m_luaEnvironment;

		OpenGLWindow *m_glWindow;
		AudioDevice m_audioDevice;

	public:


		// Initialize new game instance.
		// Creating a new OpenGL window available for rendering and initializing the entity system.
		explicit Game(
			const std::string &name,
			int width,
			int height,
			std::vector<OpenGLWindow::OpenGLAttribute> glAttributes = std::vector<OpenGLWindow::OpenGLAttribute>(),
			OpenGLVersions glVersion = OpenGLVersions::Core_3_3);
		~Game();

		void setLogging(GameLogging logging);
		void gameLoop();

		// Creating entities and scenes
		Entity& createEntity();
		Entity& createEntity(const std::string &templateName);
		Scene& createScene(const std::string &name);

		// Scene managing
		void changeScene(const std::string &sceneName);
		Scene* getActiveScene();

		// Message sending
		void sendMessage(Message *message);
		void queueMessage(Message *message);

		// Running Lua scripts
		bool runScript(const std::string &filePath);
		/*template<typename TBaseType, typename TRealType = TBaseType> void exposeComponentToLua()
		{
			auto func = [] (lua_State *state) -> int
			{
				// First argument is self
				Entity *ent = LuaEnvironment::convertUserdata<Entity>(state, 1, "jl.Entity");

				// All other args are optional parameters to constructor
				ent->addComponent<TBaseType>(new TRealType(state));

				return 0;
			};

			std::string funcName = "Add";


			lua_State *state = m_luaEnvironment.getRaw();

			// Grab component name
			TRealType tempComp = TRealType(state); // This doesn't have the args pushed, won't work
			funcName += tempComp.getName();

			const luaL_Reg funcs[] = 
			{
				{ funcName.c_str(), func },
				{ NULL, NULL }
			};


			// Grab entity metatable
			luaL_getmetatable(state, "jl.Entity");
			int metaTable = lua_gettop(state);

			luaL_setfuncs(state, funcs, 0);

			// Pop metatable value
			lua_pop(state, 1);
		};*/

		EntityPool& getEntityPool();
		SystemPool& getSystemPool();
		ScenePool& getScenePool();
		MessageCentral& getMessageCentral();
		LuaEnvironment& getLua();
		OpenGLWindow& getWindow();
	};
};

#endif