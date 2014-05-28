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


#ifndef SAUROBYTE_MESSAGE_HPP
#define SAUROBYTE_MESSAGE_HPP

#include <string>
#include <Saurobyte/IdentifierTypes.hpp>

namespace Saurobyte
{
	/*
		Message
		
		Base class for messages sent between systems
	*/
	class Entity;
	template<typename TType> class MessageData;
	struct Message
	{
		// String identifying the type of the message
		const std::string name;

		// Optional entity argument is part of all messages
		Entity *entity;

		// Type of the message data, if any
		const TypeID dataType;

		Message(const std::string &messageName, Entity *entityPtr = nullptr, TypeID typeID = TypeIdGrabber::getUniqueTypeID<Message>())
			:
			name(messageName),
			entity(entityPtr),
			dataType(typeID)
		{}

		/**
		 * Converts this message to a MessageData class and returns a copy of its data
		 * @return The custom data of this message, if any (if not, this function may yield undefined behaviour)
		 */
		template<typename TType> TType read()
		{
			return static_cast<MessageData<TType>*>(this)->data;
		};
		/**
		 * Checks if this message contains data of the specified type
		 * @return True if the message cotanins data of the type, false otherwise
		 */
		template<typename TType> bool isType()
		{
			return dataType == TypeIdGrabber::getUniqueTypeID<TType>();
		};
	};

	/*
		MessageData
		
		Generic template message used for sending arbitrary data
	*/
	template<typename TDataType> struct MessageData : public Message
	{
		TDataType data;

		MessageData(const std::string &messageName, TDataType newData, Entity *entityPtr = nullptr)
			:
			Message(messageName, entityPtr, TypeIdGrabber::getUniqueTypeID<TDataType>()),
			data(newData)
		{}
	};
};

#endif