#include "AcceptorSEH.hpp"
#include "EchoResponderSEH.hpp"
#include "EchoResponderAEH.hpp"

#include <stdexcept>

AcceptorSEH::AcceptorSEH(ListenerES::Ptr p_listenerES, HSHA& p_hsha)
  : SyncEventHandler(p_listenerES)
  , m_hsha(p_hsha)
{
}

AcceptorSEH::~AcceptorSEH()
{
}

std::string AcceptorSEH::handle(const EventSource::EventTypes& p_eventTypes)
{
  EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
  if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
  {
    ListenerES::Ptr listenerES = std::dynamic_pointer_cast<ListenerES>(m_eventSource);
    MessageES::Ptr msgES = listenerES->accept();
    EchoResponderSEH::Ptr erSEH(new EchoResponderSEH(msgES, m_hsha));
    EchoResponderAEH::Ptr erAEH(new EchoResponderAEH(msgES, m_hsha));
    m_hsha.add(HSHA::Handlers(erSEH, erAEH));
  }
  else
  {
    throw std::runtime_error("Bad event for for acceptor");
  }

  return std::string();
}
