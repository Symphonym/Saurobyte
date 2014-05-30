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

#include <Saurobyte/InputImpl.hpp>

namespace Saurobyte
{
	namespace internal
	{
		SDL_Scancode InputImpl::toSDLKey(Key key)
		{
			switch(key)
			{
				case Key::Key_0: return SDL_SCANCODE_0;
				case Key::Key_1: return SDL_SCANCODE_1;
				case Key::Key_2: return SDL_SCANCODE_2;
				case Key::Key_3: return SDL_SCANCODE_3;
				case Key::Key_4: return SDL_SCANCODE_4;
				case Key::Key_5: return SDL_SCANCODE_5;
				case Key::Key_6: return SDL_SCANCODE_6;
				case Key::Key_7: return SDL_SCANCODE_7;
				case Key::Key_8: return SDL_SCANCODE_8;
				case Key::Key_9: return SDL_SCANCODE_9;
				case Key::Numpad_0: return SDL_SCANCODE_KP_0;
				case Key::Numpad_1: return SDL_SCANCODE_KP_1;
				case Key::Numpad_2: return SDL_SCANCODE_KP_2;
				case Key::Numpad_3: return SDL_SCANCODE_KP_3;
				case Key::Numpad_4: return SDL_SCANCODE_KP_4;
				case Key::Numpad_5: return SDL_SCANCODE_KP_5;
				case Key::Numpad_6: return SDL_SCANCODE_KP_6;
				case Key::Numpad_7: return SDL_SCANCODE_KP_7;
				case Key::Numpad_8: return SDL_SCANCODE_KP_8;
				case Key::Numpad_9: return SDL_SCANCODE_KP_9;
				case Key::NumpadEnter: return SDL_SCANCODE_KP_ENTER;
				case Key::NumpadPeriod: return SDL_SCANCODE_KP_PERIOD;
				case Key::Add: return SDL_SCANCODE_KP_PLUS;
				case Key::Minus: return SDL_SCANCODE_KP_MINUS;
				case Key::Multiply: return SDL_SCANCODE_KP_MULTIPLY;
				case Key::Divide: return SDL_SCANCODE_KP_DIVIDE;
				case Key::A: return SDL_SCANCODE_A;
				case Key::B: return SDL_SCANCODE_B;
				case Key::C: return SDL_SCANCODE_C;
				case Key::D: return SDL_SCANCODE_D;
				case Key::E: return SDL_SCANCODE_E;
				case Key::F: return SDL_SCANCODE_F;
				case Key::G: return SDL_SCANCODE_G;
				case Key::H: return SDL_SCANCODE_H;
				case Key::I: return SDL_SCANCODE_I;
				case Key::J: return SDL_SCANCODE_J;
				case Key::K: return SDL_SCANCODE_K;
				case Key::L: return SDL_SCANCODE_L;
				case Key::M: return SDL_SCANCODE_M;
				case Key::N: return SDL_SCANCODE_N;
				case Key::O: return SDL_SCANCODE_O;
				case Key::P: return SDL_SCANCODE_P;
				case Key::Q: return SDL_SCANCODE_Q;
				case Key::R: return SDL_SCANCODE_R;
				case Key::S: return SDL_SCANCODE_S;
				case Key::T: return SDL_SCANCODE_T;
				case Key::U: return SDL_SCANCODE_U;
				case Key::V: return SDL_SCANCODE_V;
				case Key::W: return SDL_SCANCODE_W;
				case Key::X: return SDL_SCANCODE_X;
				case Key::Y: return SDL_SCANCODE_Y;
				case Key::Z: return SDL_SCANCODE_Z;
				case Key::Up: return SDL_SCANCODE_UP;
				case Key::Down: return SDL_SCANCODE_DOWN;
				case Key::Left: return SDL_SCANCODE_LEFT;
				case Key::Right: return SDL_SCANCODE_RIGHT;
				case Key::F1: return SDL_SCANCODE_F1;
				case Key::F2: return SDL_SCANCODE_F2;
				case Key::F3: return SDL_SCANCODE_F3;
				case Key::F4: return SDL_SCANCODE_F4;
				case Key::F5: return SDL_SCANCODE_F5;
				case Key::F6: return SDL_SCANCODE_F6;
				case Key::F7: return SDL_SCANCODE_F7;
				case Key::F8: return SDL_SCANCODE_F8;
				case Key::F9: return SDL_SCANCODE_F9;
				case Key::F10: return SDL_SCANCODE_F10;
				case Key::F11: return SDL_SCANCODE_F11;
				case Key::F12: return SDL_SCANCODE_F12;
				case Key::LCtrl: return SDL_SCANCODE_LCTRL;
				case Key::RCtrl: return SDL_SCANCODE_RCTRL;
				case Key::LAlt: return SDL_SCANCODE_LALT;
				case Key::RAlt: return SDL_SCANCODE_RALT;
				case Key::LShift: return SDL_SCANCODE_LSHIFT;
				case Key::RShift: return SDL_SCANCODE_RSHIFT;
				case Key::Semicolon: return SDL_SCANCODE_SEMICOLON;
				case Key::LBracket: return SDL_SCANCODE_LEFTBRACKET;
				case Key::RBracket: return SDL_SCANCODE_RIGHTBRACKET;
				case Key::ForwardSlash: return SDL_SCANCODE_SLASH;
				case Key::BackSlash: return SDL_SCANCODE_BACKSLASH;
				case Key::Dash: return SDL_SCANCODE_MINUS;
				case Key::Quote: return SDL_SCANCODE_APOSTROPHE;
				case Key::Equal: return SDL_SCANCODE_EQUALS;
				case Key::Period: return SDL_SCANCODE_PERIOD;
				case Key::Comma: return SDL_SCANCODE_COMMA;
				case Key::Tilde: return SDL_SCANCODE_GRAVE;
				case Key::Enter: return SDL_SCANCODE_RETURN;
				case Key::Backspace: return SDL_SCANCODE_BACKSPACE;
				case Key::Space: return SDL_SCANCODE_SPACE;
				case Key::System: return SDL_SCANCODE_APPLICATION;
				case Key::Delete: return SDL_SCANCODE_DELETE;
				case Key::Home: return SDL_SCANCODE_HOME;
				case Key::End: return SDL_SCANCODE_END;
				case Key::Escape: return SDL_SCANCODE_ESCAPE;
				case Key::Help: return SDL_SCANCODE_HELP;
				case Key::Insert: return SDL_SCANCODE_INSERT;
				case Key::PageUp: return SDL_SCANCODE_PAGEUP;
				case Key::PageDown: return SDL_SCANCODE_PAGEDOWN;
				case Key::CapsLock: return SDL_SCANCODE_CAPSLOCK;
				case Key::Numlock: return SDL_SCANCODE_NUMLOCKCLEAR;
				case Key::Scrlock: return SDL_SCANCODE_SCROLLLOCK;
				case Key::Tab: return SDL_SCANCODE_TAB;
				case Key::Pause: return SDL_SCANCODE_PAUSE;
				case Key::PrintScreen: return SDL_SCANCODE_PRINTSCREEN;
				case Key::LGui: return SDL_SCANCODE_LGUI;
				case Key::RGui: return SDL_SCANCODE_RGUI;
				case Key::Menu: return SDL_SCANCODE_MENU;

				case Key::Unknown:
				default: return SDL_SCANCODE_UNKNOWN;
			}
		}
		Key InputImpl::toSaurobyteKey(SDL_Scancode key)
		{
			switch(key)
			{
				case SDL_SCANCODE_0: return Key::Key_0;
				case SDL_SCANCODE_1: return Key::Key_1;
				case SDL_SCANCODE_2: return Key::Key_2;
				case SDL_SCANCODE_3: return Key::Key_3;
				case SDL_SCANCODE_4: return Key::Key_4;
				case SDL_SCANCODE_5: return Key::Key_5;
				case SDL_SCANCODE_6: return Key::Key_6;
				case SDL_SCANCODE_7: return Key::Key_7;
				case SDL_SCANCODE_8: return Key::Key_8;
				case SDL_SCANCODE_9: return Key::Key_9;
				case SDL_SCANCODE_KP_0: return Key::Numpad_0;
				case SDL_SCANCODE_KP_1: return Key::Numpad_1;
				case SDL_SCANCODE_KP_2: return Key::Numpad_2;
				case SDL_SCANCODE_KP_3: return Key::Numpad_3;
				case SDL_SCANCODE_KP_4: return Key::Numpad_4;
				case SDL_SCANCODE_KP_5: return Key::Numpad_5;
				case SDL_SCANCODE_KP_6: return Key::Numpad_6;
				case SDL_SCANCODE_KP_7: return Key::Numpad_7;
				case SDL_SCANCODE_KP_8: return Key::Numpad_8;
				case SDL_SCANCODE_KP_9: return Key::Numpad_9;
				case SDL_SCANCODE_KP_ENTER: return Key::NumpadEnter;
				case SDL_SCANCODE_KP_PERIOD: return Key::NumpadPeriod;
				case SDL_SCANCODE_KP_PLUS: return Key::Add;
				case SDL_SCANCODE_KP_MINUS: return Key::Minus;
				case SDL_SCANCODE_KP_MULTIPLY: return Key::Multiply;
				case SDL_SCANCODE_KP_DIVIDE: return Key::Divide;
				case SDL_SCANCODE_A: return Key::A;
				case SDL_SCANCODE_B: return Key::B;
				case SDL_SCANCODE_C: return Key::C;
				case SDL_SCANCODE_D: return Key::D;
				case SDL_SCANCODE_E: return Key::E;
				case SDL_SCANCODE_F: return Key::F;
				case SDL_SCANCODE_G: return Key::G;
				case SDL_SCANCODE_H: return Key::H;
				case SDL_SCANCODE_I: return Key::I;
				case SDL_SCANCODE_J: return Key::J;
				case SDL_SCANCODE_K: return Key::K;
				case SDL_SCANCODE_L: return Key::L;
				case SDL_SCANCODE_M: return Key::M;
				case SDL_SCANCODE_N: return Key::N;
				case SDL_SCANCODE_O: return Key::O;
				case SDL_SCANCODE_P: return Key::P;
				case SDL_SCANCODE_Q: return Key::Q;
				case SDL_SCANCODE_R: return Key::R;
				case SDL_SCANCODE_S: return Key::S;
				case SDL_SCANCODE_T: return Key::T;
				case SDL_SCANCODE_U: return Key::U;
				case SDL_SCANCODE_V: return Key::V;
				case SDL_SCANCODE_W: return Key::W;
				case SDL_SCANCODE_X: return Key::X;
				case SDL_SCANCODE_Y: return Key::Y;
				case SDL_SCANCODE_Z: return Key::Z;
				case SDL_SCANCODE_UP: return Key::Up;
				case SDL_SCANCODE_DOWN: return Key::Down;
				case SDL_SCANCODE_LEFT: return Key::Left;
				case SDL_SCANCODE_RIGHT: return Key::Right;
				case SDL_SCANCODE_F1: return Key::F1;
				case SDL_SCANCODE_F2: return Key::F2;
				case SDL_SCANCODE_F3: return Key::F3;
				case SDL_SCANCODE_F4: return Key::F4;
				case SDL_SCANCODE_F5: return Key::F5;
				case SDL_SCANCODE_F6: return Key::F6;
				case SDL_SCANCODE_F7: return Key::F7;
				case SDL_SCANCODE_F8: return Key::F8;
				case SDL_SCANCODE_F9: return Key::F9;
				case SDL_SCANCODE_F10: return Key::F10;
				case SDL_SCANCODE_F11: return Key::F11;
				case SDL_SCANCODE_F12: return Key::F12;
				case SDL_SCANCODE_LCTRL: return Key::LCtrl;
				case SDL_SCANCODE_RCTRL: return Key::RCtrl;
				case SDL_SCANCODE_LALT: return Key::LAlt;
				case SDL_SCANCODE_RALT: return Key::RAlt;
				case SDL_SCANCODE_LSHIFT: return Key::LShift;
				case SDL_SCANCODE_RSHIFT: return Key::RShift;
				case SDL_SCANCODE_SEMICOLON: return Key::Semicolon;
				case SDL_SCANCODE_LEFTBRACKET: return Key::LBracket;
				case SDL_SCANCODE_RIGHTBRACKET: return Key::RBracket;
				case SDL_SCANCODE_SLASH: return Key::ForwardSlash;
				case SDL_SCANCODE_BACKSLASH: return Key::BackSlash;
				case SDL_SCANCODE_MINUS: return Key::Dash;
				case SDL_SCANCODE_APOSTROPHE: return Key::Quote;
				case SDL_SCANCODE_EQUALS: return Key::Equal;
				case SDL_SCANCODE_PERIOD: return Key::Period;
				case SDL_SCANCODE_COMMA: return Key::Comma;
				case SDL_SCANCODE_GRAVE: return Key::Tilde;
				case SDL_SCANCODE_RETURN: return Key::Enter;
				case SDL_SCANCODE_BACKSPACE: return Key::Backspace;
				case SDL_SCANCODE_SPACE: return Key::Space;
				case SDL_SCANCODE_APPLICATION: return Key::System;
				case SDL_SCANCODE_DELETE: return Key::Delete;
				case SDL_SCANCODE_HOME: return Key::Home;
				case SDL_SCANCODE_END: return Key::End;
				case SDL_SCANCODE_ESCAPE: return Key::Escape;
				case SDL_SCANCODE_HELP: return Key::Help;
				case SDL_SCANCODE_INSERT: return Key::Insert;
				case SDL_SCANCODE_PAGEUP: return Key::PageUp;
				case SDL_SCANCODE_PAGEDOWN: return Key::PageDown;
				case SDL_SCANCODE_CAPSLOCK: return Key::CapsLock;
				case SDL_SCANCODE_NUMLOCKCLEAR: return Key::Numlock;
				case SDL_SCANCODE_SCROLLLOCK: return Key::Scrlock;
				case SDL_SCANCODE_TAB: return Key::Tab;
				case SDL_SCANCODE_PAUSE: return Key::Pause;
				case SDL_SCANCODE_PRINTSCREEN: return Key::PrintScreen;
				case SDL_SCANCODE_LGUI: return Key::LGui;
				case SDL_SCANCODE_RGUI: return Key::RGui;
				case SDL_SCANCODE_MENU: return Key::Menu;

				case SDL_SCANCODE_UNKNOWN:
				default: return Key::Unknown;
			}
		}
	}

};