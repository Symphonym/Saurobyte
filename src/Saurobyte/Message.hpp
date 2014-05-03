#ifndef JL_MESSAGE_HPP
#define JL_MESSAGE_HPP

#include <string>
#include <Saurobyte/IdentifierTypes.hpp>

namespace jl
{
	/*
		Message
		
		Base class for messages sent between systems
	*/
	class Entity;
	struct Message
	{
		// String identifying the type of the message
		const std::string name;

		// Optional entity argument is part of all messages
		Entity *entity;

		// Type of the message data, if any
		const TypeID dataType;

		Message(const std::string &messageName, TypeID typeID = 0, Entity *entityPtr = nullptr)
			:
			name(messageName),
			entity(entityPtr),
			dataType(typeID)
		{}

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
			Message(messageName, TypeIdGrabber::getUniqueTypeID<TDataType>()),
			data(newData)
		{}
	};

	/*
		createMessage
		
		Default way of creating messages to send, hides the new allocation from the user
		as well as providing an easy-to-use way to create messages.
	*/
	template<typename TType = char> Message* createMessage(
		const std::string &messageName,
		TType newData = 0,
		Entity *entityPtr = nullptr)
	{
		return new MessageData<TType>(messageName, newData, entityPtr);
	}
};

#endif