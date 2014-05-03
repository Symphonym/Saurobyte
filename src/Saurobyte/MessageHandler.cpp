#include <Saurobyte/MessageHandler.hpp>
#include <Saurobyte/MessageCentral.hpp>
#include <Saurobyte/Message.hpp>

namespace jl
{
		MessageHandler::MessageHandler(MessageCentral *center)
			:
			m_center(center)
		{

		}
		MessageHandler::~MessageHandler()
		{
			if(m_center != nullptr)
				m_center->unsubscribeAll(this);
		}

		void MessageHandler::subscribe(const std::string &messageName)
		{
			if(m_center != nullptr)
				m_center->subscribe(messageName, this);
		}
		void MessageHandler::unsubscribe(const std::string &messageName)
		{
			if(m_center != nullptr)
				m_center->unsubscribe(messageName, this);
		}

		void MessageHandler::sendMessage(Message *message)
		{
			if(m_center != nullptr)
				m_center->sendMessage(message);
		}
		void MessageHandler::queueMessage(Message *message)
		{
			if(m_center != nullptr)
				m_center->queueMessage(message);
		}

		bool MessageHandler::subscribedTo(const std::string &messageName) const
		{

			return m_center == nullptr ? false : m_center->subscribedTo(messageName, this);
		}
};