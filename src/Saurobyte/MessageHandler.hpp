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


#ifndef SAUROBYTE_MESSAGE_HANDLER_HPP
#define SAUROBYTE_MESSAGE_HANDLER_HPP

#include <Saurobyte/ApiDefines.hpp>
#include <Saurobyte/Message.hpp>
#include <string>
#include <unordered_set>

namespace Saurobyte
{
	class MessageCentral;
	class SAUROBYTE_API MessageHandler
	{
	public:

		/**
		 * Handler function for dealing with received messages
		 * @param message The received message
		 */
		virtual void onMessage(const Message &message) {};

		/**
		 * Subscribe this handler to messages of the specified name
		 * @param messageName The message name to subscribe to
		 */
		void subscribe(const std::string &messageName);

		/**
		 * Unsubscribe this handler from messages of the specified type, only has an effect if the handler is already subscribed
		 * @param messageName The message name to subscribe to
		 */
		void unsubscribe(const std::string &messageName);

		/**
		 * Broadcasts a message through the central to all subscribers of the message
		 * @param message The message to send
		 */
		void sendMessage(const Message &message);
		/**
		 * Broadcasts a message through the central to all subscribers of the message
		 * @param messageName The name/title of the message, used for identifying it
		 * @param entity      Optional entity pointer to be sent with the message
		 */
		void sendMessage(const std::string &messageName, Entity *entity = nullptr);
		/**
		 * Broadcasts a message through the central to all subscribers of the message
		 * @param messageName The name/title of the message, used for identifying it
		 * @param data        Data to be sent with the message
		 * @param entity      Optional entity pointer to be sent with the message
		 */
		template<typename TType> void sendMessage(const std::string &messageName, TType data, Entity *entity = nullptr)
		{
			sendMessage(MessageData<TType>(messageName, data, entity));
		};
		
		/**
		 * Checks if this handler is subscribed to the specified message
		 * @param  messageName The message identifier to check
		 * @return             Whether or not this handler subscribes to the message
		 */
		bool subscribedTo(const std::string &messageName) const;

	protected:

		MessageHandler(MessageCentral *center);
		~MessageHandler();

	private:

		// So the central can disconnect all handlers on destruction
		// and store subscriptions locally
		friend class MessageCentral;

		// Keep a local register of subscriptions for easy lookup
		std::unordered_set<std::string> m_subscriptions;

		MessageCentral *m_center;

	};
};

#endif