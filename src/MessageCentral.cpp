#include "MessageCentral.hpp"
#include "MessageHandler.hpp"
#include "Message.hpp"

namespace jl
{
		MessageCentral::MessageCentral()
		{

		}
		MessageCentral::~MessageCentral()
		{
			// Nullify center pointers of all handlers, so they don't get null error when center is destroyed
			for(auto itr = m_subscriptionCentral.begin(); itr != m_subscriptionCentral.end(); itr++)
			{
				std::vector<MessageHandler*>& listeners = itr->second;
				for(std::size_t i = 0; i < listeners.size(); i++)
				{
					listeners[i]->m_subscriptions.clear();
					listeners[i]->m_center = nullptr;
				}
			}
			m_subscriptionCentral.clear();
		}

		void MessageCentral::subscribe(const std::string &messageName, MessageHandler *handler)
		{
			if(subscribedTo(messageName, handler))
				return;

			handler->m_subscriptions.insert(messageName); // Store local subscription info in the handler
			m_subscriptionCentral[messageName].push_back(handler);
		}
		void MessageCentral::unsubscribe(const std::string &messageName, MessageHandler *handler)
		{
			std::vector<MessageHandler*>& listeners = m_subscriptionCentral[messageName];
			if(subscribedTo(messageName, handler))
			{
				// Remove local subscription info
				handler->m_subscriptions.erase(messageName);
				for(std::size_t i = 0; i < listeners.size(); i++)
				{
					if(listeners[i] == handler)
					{
						listeners.erase(listeners.begin() + i);
						break;
					}
				}
			}
		}
		void MessageCentral::unsubscribeAll(MessageHandler *handler)
		{
			for(auto itr = handler->m_subscriptions.begin(); itr != handler->m_subscriptions.end(); itr++)
			{
				std::vector<MessageHandler*>& listeners = m_subscriptionCentral[*itr];
				for(std::size_t i = 0; i < listeners.size(); i++)
				{
					if(listeners[i] == handler)
					{
						listeners.erase(listeners.begin() + i);
						break;
					}
				}
			}

			// Remove local subscription info
			handler->m_subscriptions.clear();
		}

		void MessageCentral::broadcast(Message *message)
		{
			auto iter = m_subscriptionCentral.find(message->name);
			if(iter != m_subscriptionCentral.end())
			{
				for(std::size_t i = 0; i < iter->second.size(); i++)
					iter->second[i]->onMessage(message);
			}
			delete message;
		}

		bool MessageCentral::subscribedTo(const std::string &messageName, const MessageHandler *handler) const
		{
			return handler->m_subscriptions.find(messageName) != handler->m_subscriptions.end();
		}
};