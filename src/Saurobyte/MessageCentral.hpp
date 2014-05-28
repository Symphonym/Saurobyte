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


#ifndef SAUROBYTE_MESSAGE_CENTER_HPP
#define SAUROBYTE_MESSAGE_CENTER_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/Message.hpp>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace Saurobyte
{
	class MessageHandler;
	class SAUROBYTE_API MessageCentral
	{

	public:

		MessageCentral();
		~MessageCentral();
		
		// Subscribe the specified handler to messages of the specified name
		void subscribe(const std::string &messageName, MessageHandler *handler);

		// Unsubscribe the specified handler from messages of the specified name
		void unsubscribe(const std::string &messageName, MessageHandler *handler);
		// Unsubscribe the specified handler from all messages it's currently subscribed to
		// , used primarily for cleanup.
		void unsubscribeAll(MessageHandler *handler);

		void sendQueuedMessages();

		// Broadcast message instantly, sending it to those who subscribes to messages of its type
		void sendMessage(const Message &message);
		void sendMessage(const std::string &messageName, Entity *entity = nullptr);
		template<typename TType> void sendMessage(const std::string &messageName, TType data, Entity *entity = nullptr)
		{
			sendMessage(MessageData<TType>(messageName, data, entity));
		};
		// Queue message, broadcasting it whenever the message queue is sent
		void queueMessage(Message *message);

		// Checks whether or not the specified handler is subscribed to the specified message
		bool subscribedTo(const std::string &messageName, const MessageHandler *handler) const;

	private:

		typedef std::unordered_map<std::string, std::vector<MessageHandler*> > MessageSubscriptions;
		typedef std::vector<Message*> MessageQueue;

		MessageSubscriptions m_subscriptionCentral;
		MessageQueue m_messageQueue;

	};
};

#endif