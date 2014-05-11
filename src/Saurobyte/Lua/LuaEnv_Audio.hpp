#ifndef JL_LUAENV_AUDIO_HPP
#define JL_LUAENV_AUDIO_HPP

class lua_State;
namespace Saurobyte
{
	class Game;
	class LuaEnv_Audio
	{

	private:

		// Play static sound
		static int PlaySound(lua_State *state);

		// Play streaming sound
		static int PlayStream(lua_State *state);

		// Register an audio file to a filepath
		static int RegisterAudio(lua_State *state);
		
	public:
		
		static void exposeToLua(Game *game);
	};
};

#endif