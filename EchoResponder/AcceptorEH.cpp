#include "AcceptorEH.hpp"
#include "EchoResponderEH.hpp"
#include "../IO/Epoll.hpp"
#include "../EventEngines/Reactor.hpp"

#include <stdexcept>

AcceptorEH::AcceptorEH(ListenerES::Ptr p_listenerES, Reactor& p_reactor)
  : EventHandler(p_listenerES)
  , m_reactor(p_reactor)
{
}

AcceptorEH::~AcceptorEH()
{
}

void AcceptorEH::handle(const EventSource::EventTypes& p_eventTypes)
{
  EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
  if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
  {
    ListenerES::Ptr listenerES = boost::dynamic_pointer_cast<ListenerES>(m_eventSource);
    MessageES::Ptr msgES = listenerES->accept();
    EchoResponderEH::Ptr erEH(new EchoResponderEH(msgES, m_reactor));
    m_reactor.add(erEH);
  }
  else
  {
    throw std::runtime_error("Bad event for for acceptor");
  }
}
