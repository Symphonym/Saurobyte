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


#ifndef SAUROBYTE_EVENT_HPP
#define SAUROBYTE_EVENT_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/Input.hpp>

namespace Saurobyte
{
	class Window;

	struct Event {};
	struct SAUROBYTE_API WindowEvent : public Event
	{
		Window &window;

		explicit WindowEvent(Window &windowRef);
	};
	struct SAUROBYTE_API WindowSizeEvent : public WindowEvent
	{
		unsigned int width;
		unsigned int height;

		explicit WindowSizeEvent(Window &window, unsigned int newWidth, unsigned int newHeight);
	};

	struct SAUROBYTE_API WindowMoveEvent : public WindowEvent
	{
		int x;
		int y;

		explicit WindowMoveEvent(Window &window, int newX, int newY);
	};

	struct SAUROBYTE_API KeyEvent : public Event
	{
		Key key;
		bool pressed;
		bool keyRepeat;

		explicit KeyEvent(Key eventKey, bool isPressed, bool isKeyRepeat);
	};

	struct SAUROBYTE_API MouseEvent : public Event
	{
		int x;
		int y;
		bool touchGenerated;

		explicit MouseEvent(int posX, int posY, bool touchClick);
	};

	struct SAUROBYTE_API MouseButtonEvent : public MouseEvent
	{
		MouseButton button;
		bool pressed;
		unsigned int clicks;

		explicit MouseButtonEvent(
			int posX,
			int posY,
			bool touchClick,
			MouseButton clickButton,
			bool isPressed,
			unsigned int clickCount);
	};
};

#endif