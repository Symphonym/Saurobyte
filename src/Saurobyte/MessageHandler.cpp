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

#include <Saurobyte/MessageHandler.hpp>
#include <Saurobyte/MessageCentral.hpp>
#include <Saurobyte/Message.hpp>

namespace Saurobyte
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
		
		void MessageHandler::sendMessage(const Message &message)
		{
			if(m_center != nullptr)
				m_center->sendMessage(message);
		}
		void MessageHandler::sendMessage(const std::string &messageName, Entity *entity)
		{
			sendMessage(Message(messageName, entity));
		}

		bool MessageHandler::subscribedTo(const std::string &messageName) const
		{

			return m_center == nullptr ? false : m_center->subscribedTo(messageName, this);
		}

};