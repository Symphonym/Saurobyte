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
		SDL_Keycode keyCode;

		switch(key)
		{
			case Key::Key_0: keyCode = SDLK_0; break;
			case Key::Key_1: keyCode = SDLK_1; break;
			case Key::Key_2: keyCode = SDLK_2; break;
			case Key::Key_3: keyCode = SDLK_3; break;
			case Key::Key_4: keyCode = SDLK_4; break;
			case Key::Key_5: keyCode = SDLK_5; break;
			case Key::Key_6: keyCode = SDLK_6; break;
			case Key::Key_7: keyCode = SDLK_7; break;
			case Key::Key_8: keyCode = SDLK_8; break;
			case Key::Key_9: keyCode = SDLK_9; break;
			case Key::Numpad_0: keyCode = SDLK_KP_0; break;
			case Key::Numpad_1: keyCode = SDLK_KP_1; break;
			case Key::Numpad_2: keyCode = SDLK_KP_2; break;
			case Key::Numpad_3: keyCode = SDLK_KP_3; break;
			case Key::Numpad_4: keyCode = SDLK_KP_4; break;
			case Key::Numpad_5: keyCode = SDLK_KP_5; break;
			case Key::Numpad_6: keyCode = SDLK_KP_6; break;
			case Key::Numpad_7: keyCode = SDLK_KP_7; break;
			case Key::Numpad_8: keyCode = SDLK_KP_8; break;
			case Key::Numpad_9: keyCode = SDLK_KP_9; break;
			case Key::A: keyCode = SDLK_a; break;
			case Key::B: keyCode = SDLK_b; break;
			case Key::C: keyCode = SDLK_c; break;
			case Key::D: keyCode = SDLK_d; break;
			case Key::E: keyCode = SDLK_e; break;
			case Key::F: keyCode = SDLK_f; break;
			case Key::G: keyCode = SDLK_g; break;
			case Key::H: keyCode = SDLK_h; break;
			case Key::I: keyCode = SDLK_i; break;
			case Key::J: keyCode = SDLK_j; break;
			case Key::K: keyCode = SDLK_k; break;
			case Key::L: keyCode = SDLK_l; break;
			case Key::M: keyCode = SDLK_m; break;
			case Key::N: keyCode = SDLK_n; break;
			case Key::O: keyCode = SDLK_o; break;
			case Key::P: keyCode = SDLK_p; break;
			case Key::Q: keyCode = SDLK_q; break;
			case Key::R: keyCode = SDLK_r; break;
			case Key::S: keyCode = SDLK_s; break;
			case Key::T: keyCode = SDLK_t; break;
			case Key::U: keyCode = SDLK_u; break;
			case Key::V: keyCode = SDLK_v; break;
			case Key::W: keyCode = SDLK_w; break;
			case Key::X: keyCode = SDLK_x; break;
			case Key::Y: keyCode = SDLK_y; break;
			case Key::Z: keyCode = SDLK_z; break;
			case Key::Up: keyCode = SDLK_UP; break;
			case Key::Down: keyCode = SDLK_DOWN; break;
			case Key::Left: keyCode = SDLK_LEFT; break;
			case Key::Right: keyCode = SDLK_RIGHT; break;
			case Key::F1: keyCode = SDLK_F1; break;
			case Key::F2: keyCode = SDLK_F2; break;
			case Key::F3: keyCode = SDLK_F3; break;
			case Key::F4: keyCode = SDLK_F4; break;
			case Key::F5: keyCode = SDLK_F5; break;
			case Key::F6: keyCode = SDLK_F6; break;
			case Key::F7: keyCode = SDLK_F7; break;
			case Key::F8: keyCode = SDLK_F8; break;
			case Key::F9: keyCode = SDLK_F9; break;
			case Key::F10: keyCode = SDLK_F10; break;
			case Key::F11: keyCode = SDLK_F11; break;
			case Key::F12: keyCode = SDLK_F12; break;
			case Key::LCtrl: keyCode = SDLK_LCTRL; break;
			case Key::RCtrl: keyCode = SDLK_RCTRL; break;
			case Key::LAlt: keyCode = SDLK_LALT; break;
			case Key::RAlt: keyCode = SDLK_RALT; break;
			case Key::LShift: keyCode = SDLK_LSHIFT; break;
			case Key::RShift: keyCode = SDLK_RSHIFT; break;
			case Key::Colon: keyCode = SDLK_COLON; break;
			case Key::Semicolon: keyCode = SDLK_SEMICOLON; break;
			case Key::LBracket: keyCode = SDLK_LEFTBRACKET; break;
			case Key::RBracket: keyCode = SDLK_RIGHTBRACKET; break;
			case Key::ForwardSlash: keyCode = SDLK_SLASH; break;
			case Key::BackSlash: keyCode = SDLK_BACKSLASH; break;
			case Key::Ampersand: keyCode = SDLK_AMPERSAND; break;
			case Key::Asterisk: keyCode = SDLK_ASTERISK; break;
			case Key::At: keyCode = SDLK_AT; break;
			case Key::Caret: keyCode = SDLK_CARET; break;
			case Key::Dollar: keyCode = SDLK_DOLLAR; break;
			case Key::Exclaimation: keyCode = SDLK_EXCLAIM; break;
			case Key::RArrow: keyCode = SDLK_GREATER; break;
			case Key::LArrow: keyCode = SDLK_LESS; break;
			case Key::Hash: keyCode = SDLK_HASH; break;
			case Key::Percent: keyCode = SDLK_PERCENT; break;
			case Key::Plus: keyCode = SDLK_PLUS; break;
			case Key::Minus: keyCode = SDLK_MINUS; break;
			case Key::Question: keyCode = SDLK_QUESTION; break;
			case Key::Quote: keyCode = SDLK_QUOTE; break;
			case Key::DoubleQuote: keyCode = SDLK_QUOTEDBL; break;
			case Key::LParen: keyCode = SDLK_LEFTPAREN; break;
			case Key::RParen: keyCode = SDLK_RIGHTPAREN; break;
			case Key::Underscore: keyCode = SDLK_UNDERSCORE; break;
			case Key::Equal: keyCode = SDLK_EQUALS; break;
			case Key::Period: keyCode = SDLK_PERIOD; break;
			case Key::Comma: keyCode = SDLK_COMMA; break;
			case Key::Enter: keyCode = SDLK_RETURN; break;
			case Key::Backspace: keyCode = SDLK_BACKSPACE; break;
			case Key::Space: keyCode = SDLK_SPACE; break;
			case Key::System: keyCode = SDLK_APPLICATION; break;
			case Key::Delete: keyCode = SDLK_DELETE; break;
			case Key::Home: keyCode = SDLK_HOME; break;
			case Key::End: keyCode = SDLK_END; break;
			case Key::Escape: keyCode = SDLK_ESCAPE; break;
			case Key::Help: keyCode = SDLK_HELP; break;
			case Key::Insert: keyCode = SDLK_INSERT; break;
			case Key::PageUp: keyCode = SDLK_PAGEUP; break;
			case Key::PageDown: keyCode = SDLK_PAGEDOWN; break;
			case Key::CapsLock: keyCode = SDLK_CAPSLOCK; break;
			case Key::Numlock: keyCode = SDLK_NUMLOCKCLEAR; break;
			case Key::Scrlock: keyCode = SDLK_SCROLLLOCK; break;
			case Key::Tab: keyCode = SDLK_TAB; break;
			case Key::Pause: keyCode = SDLK_PAUSE; break;
			case Key::PrintScreen: keyCode = SDLK_PRINTSCREEN; break;
			default: keyCode = SDLK_UNKNOWN; break;
		}

		return SDL_GetKeyboardState(NULL)[keyCode];
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