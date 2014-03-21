#ifndef JL_MESSAGECENTER_HPP
#define JL_MESSAGECENTER_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace jl
{
	class Message;
	class MessageHandler;
	class MessageCentral
	{
		private:

		typedef std::unordered_map<std::string, std::vector<MessageHandler*> > MessageSubscriptions;

		MessageSubscriptions m_subscriptionCentral;

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

		// Broadcast message, sending it to those who subscribes to messages of its type
		void broadcast(Message *message);

		// Checks whether or not the specified handler is subscribed to the specified message
		bool subscribedTo(const std::string &messageName, const MessageHandler *handler) const;
	};
};

#endif