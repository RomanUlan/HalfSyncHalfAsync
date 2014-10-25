#include "EchoResponderSEH.hpp"
#include "../IO/Epoll.hpp"

#include <boost/algorithm/string.hpp>

EchoResponderSEH::EchoResponderSEH(MessageES::Ptr p_messageES/*, Reactor& p_reactor*/)
	: SyncEventHandler(p_messageES)//, m_reactor(p_reactor)
{
}

EchoResponderSEH::~EchoResponderSEH()
{
}

std::string EchoResponderSEH::handle(const EventSource::EventTypes& p_eventTypes)
{
	std::string data;
	EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
	if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
	{
		MessageES::Ptr mES = std::dynamic_pointer_cast <MessageES> (m_eventSource);
		//MessageES::Ptr mes; m_eventSource = std::dynamic_pointer_cast<SocketES>(mes);
		while (!boost::ends_with(data, "\n"))
		{
			std::string part;
			mES->read(part);
			data.append(part);
		}
		mES->getSocket()->write(data);
	}
	else
	{
		//m_reactor.remove(m_eventSource->getDescriptor());
	}

	return data;
}
