#ifndef JL_MESSAGECENTER_HPP
#define JL_MESSAGECENTER_HPP

#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace Saurobyte
{
	class Message;
	class MessageHandler;
	class MessageCentral
	{
		private:

		typedef std::unordered_map<std::string, std::vector<MessageHandler*> > MessageSubscriptions;
		typedef std::vector<Message*> MessageQueue;

		MessageSubscriptions m_subscriptionCentral;
		MessageQueue m_messageQueue;

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
		void sendMessage(Message *message);
		// Queue message, broadcasting it whenever the message queue is sent
		void queueMessage(Message *message);

		// Checks whether or not the specified handler is subscribed to the specified message
		bool subscribedTo(const std::string &messageName, const MessageHandler *handler) const;
	};
};

#endif