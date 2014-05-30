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


#ifndef SAUROBYTE_INPUT_IMPL_HPP
#define SAUROBYTE_INPUT_IMPL_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/Input.hpp>
#include <SDL2/SDL_scancode.h>

namespace Saurobyte
{
	namespace internal
	{
		class SAUROBYTE_API InputImpl
		{
		public:

			/**
			 * Converts the given Saurobyte key type to an SDL_Scancode key type
			 * @param  key The Saurobyte key
			 * @return     The key in SDL_Scancode format
			 */
			static SDL_Scancode toSDLKey(Key key);
			/**
			 * Converts the given SDL_Scancode key to the same key in Saurobyte key format
			 * @param  key The SDL_Scancode key
			 * @return     The key in Saurobyte format
			 */
			static Key toSaurobyteKey(SDL_Scancode key);
		};	
	}

};


#endif