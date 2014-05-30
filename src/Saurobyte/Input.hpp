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


#ifndef SAUROBYTE_INPUT_HPP
#define SAUROBYTE_INPUT_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/Math/Vector2.hpp>
#include <vector>

namespace Saurobyte
{
	enum class MouseButton
	{
		Left = 1,
		Middle = 2,
		Right = 3
	};

	enum class Key
	{
		/**
		 * Number/numpad keys
		 */
		Key_0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Numpad_0,
		Numpad_1,
		Numpad_2,
		Numpad_3,
		Numpad_4,
		Numpad_5,
		Numpad_6,
		Numpad_7,
		Numpad_8,
		Numpad_9,
		NumpadEnter,
		NumpadPeriod,
		Add,
		Minus,
		Multiply,
		Divide,

		/**
		 * Character keys
		 */
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		/**
		 * Direction keys
		 */
		Up,
		Down,
		Left,
		Right,

		/**
		 * Function keys
		 */
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		/**
		 * Modifier keys
		 */
		LCtrl,
		RCtrl,
		LAlt,
		RAlt,
		RShift,
		LShift,

		/**
		 * Symbol keys
		 */
		Colon,
		Semicolon,
		LBracket,
		RBracket,
		ForwardSlash,
		BackSlash,
		Dash,
		Quote,
		Equal,
		Period,
		Comma,
		Tilde,

		/**
		 * Misc keys
		 */
		Enter,
		Backspace,
		Space,
		System, // The "Windows" key
		Delete,
		Home,
		End,
		Escape,
		Help,
		Insert,
		PageUp,
		PageDown,
		CapsLock,
		Numlock,
		Scrlock,
		Tab,
		Pause,
		PrintScreen,
		LGui,
		RGui,
		Menu,


		/**
		 * Unknown key
		 */
		Unknown


	};

	class SAUROBYTE_API Input
	{
	public:

		/**
		 * Checks the current state of a mouse button
		 * @param  button The button to check
		 * @return        True if the button is pressed, false otherwise
		 */
		static bool isMousePressed(MouseButton button);
		/**
		 * Checks the current state of a keyboard key
		 * @param  key The key to check
		 * @return     True if the key is pressed, false otherwise
		 */
		static bool isKeyPressed(Key key);

		/**
		 * Sets the visible of the cursor
		 * @param visible Whether or not the cursor should be visible
		 */
		static void setCursorVisible(bool visible);

		/**
		 * Queries the current keyboard state and grabs all keys that are currently pressed
		 * @return A vector of all currently pressed keys
		 */
		static std::vector<Key> getPressedKeys();
		/**
		 * Queries the screen coordinates of the mouse cursor
		 * @return Position of the mouse
		 */
		static Vector2i getMousePosition();
		/**
		 * Returns the delta vector since the last time this function was called
		 * @return Mouse delta since last call to this function
		 */
		static Vector2i getMouseDelta();
		/**
		 * Check if the mouse cursor is currently visible
		 * @return True if the cursor is visible, false otherwise
		 */
		static bool getCursorVisible();

	};
};


#endif