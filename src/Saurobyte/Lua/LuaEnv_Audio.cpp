
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

#include <Saurobyte/Lua/LuaEnv_Audio.hpp>
#include <Saurobyte/LuaEnvironment.hpp>
#include <Saurobyte/AudioDevice.hpp>
#include <Saurobyte/Engine.hpp>

namespace Saurobyte
{
	int LuaEnv_Audio::PlaySound(LuaEnvironment &env)
	{
		// First arg is name of sound
		std::string soundName = env.readArg<std::string>();

		AudioHandle sound = AudioDevice::playSound(soundName);

		if(sound->isValid())
			env.pushArgs(true);
		else
			env.pushArgs(false);

		return 1;
	}
	int LuaEnv_Audio::PlayStream(LuaEnvironment &env)
	{
		// First arg is name of sound
		std::string soundName = env.readArg<std::string>();

		AudioHandle sound = AudioDevice::playStream(soundName);

		if(sound->isValid())
			env.pushArgs(true);
		else
			env.pushArgs(false);

		return 1;
	}
	int LuaEnv_Audio::RegisterAudio(LuaEnvironment &env)
	{
		// First arg filepath of the sound
		std::string filePath = env.readArg<std::string>();

		// Second arg is the new name of the sound
		std::string soundName = env.readArg<std::string>();

		AudioDevice::registerAudio(filePath, soundName);
	}

	void LuaEnv_Audio::exposeToLua(Engine *engine)
	{
		LuaEnvironment &env = engine->getLua();
		env.registerFunction({"PlaySound", PlaySound});
		env.registerFunction({"PlayStream", PlayStream});
		env.registerFunction({"RegisterAudio", RegisterAudio});

		/*const luaL_Reg audioFuncs[] = 
		{
			{ "PlaySound", PlaySound },
			{ "PlayStream", PlayStream },
			{ "RegisterAudio", RegisterAudio },
			{ NULL, NULL }
		};

		// Register sound functions at global scope
		LuaEnvironment &env = engine->getLua().getRaw();
		lua_pushglobaltable(state);
		luaL_setfuncs (state, audioFuncs, 0);*/
	}
};