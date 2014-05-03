#ifndef JL_LUAENV_ENTITY_HPP
#define JL_LUAENV_ENTITY_HPP


class lua_State;
namespace jl
{
	class Game;
	class LuaEnv_Entity
	{

	private:

		// Add component to entity
		//static int AddComponent = [] (lua_State *state);

		// Create a component TODO with creatinf and add n shit
		//static int CreateComponent = [] (lua_State *state);

		// Getter for components themselves
		static int GetComponent(lua_State *state);

		// Get component count of entity
		static int GetComponentCount(lua_State *state);

		// Remove components
		static int RemoveComponent(lua_State *state);

		// Has component
		static int HasComponent(lua_State *state);

		// Enable entity
		static int EnableEntity(lua_State *state);

		// Disable entity
		static int DisableEntity(lua_State *state);

		// Kill entity
		static int KillEntity(lua_State *state);

		// Get ID of entity
		static int GetID(lua_State *state);


		// Subscribe entity to an event
		static int SubscribeEvent(lua_State *state);

		// Unsubscribe entity to an event
		static int UnsubscribeEvent(lua_State *state);

	public:

		static void exposeToLua(Game *game);
	};
};

#endif