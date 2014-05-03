#ifndef JL_MESSAGEHANDLER_HPP
#define JL_MESSAGEHANDLER_HPP

#include <string>
#include <unordered_set>

namespace jl
{
	class Message;
	class MessageCentral;
	class MessageHandler
	{
	private:

		friend class MessageCentral;

		// Keep a local register of subscriptions for easy lookup
		std::unordered_set<std::string> m_subscriptions;

		MessageCentral *m_center;

	protected:

		MessageHandler(MessageCentral *center);
		~MessageHandler();

	public:
		
		// Subscribe this handler to messages of the specified name
		void subscribe(const std::string &messageName);

		// Unsubscribe this handler from messages of the specified name
		void unsubscribe(const std::string &messageName);

		virtual void onMessage(Message *message) {};

		// Broadcast message to all subscribers
		void sendMessage(Message *message);
		void queueMessage(Message *message);

		// Checks whether or not you're subscribed to the specified message
		bool subscribedTo(const std::string &messageName) const;
	};
};

#endif