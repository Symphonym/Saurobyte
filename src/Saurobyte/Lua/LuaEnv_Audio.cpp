#include <Saurobyte/Lua/LuaEnv_Audio.hpp>
#include <Saurobyte/Game.hpp>

namespace jl
{
	int LuaEnv_Audio::PlaySound(lua_State *state)
	{
		// First arg is name of sound
		std::string soundName = luaL_checkstring(state, 1);

		AudioHandle sound = AudioDevice::playSound(soundName);

		if(sound)
			lua_pushboolean(state, true);
		else
			lua_pushboolean(state, false);

		return 1;
	}
	int LuaEnv_Audio::PlayStream(lua_State *state)
	{
		// First arg is name of sound
		std::string soundName = luaL_checkstring(state, 1);

		AudioHandle sound = AudioDevice::playStream(soundName);

		if(sound)
			lua_pushboolean(state, true);
		else
			lua_pushboolean(state, false);

		return 1;
	}
	int LuaEnv_Audio::RegisterAudio(lua_State *state)
	{
		// First arg filepath of the sound
		std::string filePath = luaL_checkstring(state, 1);

		// Second arg is the new name of the sound
		std::string soundName = luaL_checkstring(state, 2);

		AudioDevice::registerAudio(filePath, soundName);
	}

	void LuaEnv_Audio::exposeToLua(Game *game)
	{
		const luaL_Reg audioFuncs[] = 
		{
			{ "PlaySound", PlaySound },
			{ "PlayStream", PlayStream },
			{ "RegisterAudio", RegisterAudio },
			{ NULL, NULL }
		};

		// Register sound functions at global scope
		lua_State *state = game->getLua().getRaw();
		lua_pushglobaltable(state);
		luaL_setfuncs (state, audioFuncs, 0);
	}
};