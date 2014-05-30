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


#include <Saurobyte/Input.hpp>
#include <Saurobyte/InputImpl.hpp>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_keyboard.h>

namespace Saurobyte
{
	bool Input::isMousePressed(MouseButton button)
	{
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(static_cast<int>(button));
	}
	bool Input::isKeyPressed(Key key)
	{
		return SDL_GetKeyboardState(NULL)[internal::InputImpl::toSDLKey(key)];
	}

	std::vector<Key> Input::getPressedKeys()
	{
		int numKeys = 0;
		const Uint8 *keyboardState = SDL_GetKeyboardState(&numKeys);
		std::vector<Key> pressedKeys;

		for(int i = 0; i < numKeys; i++)
		{
			Key key = internal::InputImpl::toSaurobyteKey(static_cast<SDL_Scancode>(keyboardState[i]));

			if(key != Key::Unknown)
				pressedKeys.push_back(key);
		}

		return pressedKeys;
	}
	Vector2i Input::getMousePosition()
	{
		Vector2i vec;
		SDL_GetMouseState(&vec.x, &vec.y);
		return vec;
	}
	Vector2i Input::getMouseDelta()
	{
		Vector2i vec;
		SDL_GetRelativeMouseState(&vec.x, &vec.y);
		return vec;
	}

	void Input::setCursorVisible(bool visible)
	{
		SDL_ShowCursor(visible ? 1 : 0);
	}
	bool Input::getCursorVisible()
	{
		return SDL_ShowCursor(-1);
	}
}