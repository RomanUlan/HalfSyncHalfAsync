#include "EchoResponderAEH.hpp"

EchoResponderAEH::EchoResponderAEH(MessageES::Ptr p_socket, HSHA& p_hsha)
  : AsyncEventHandler(p_socket), m_hsha(p_hsha)
{
}

EchoResponderAEH::~EchoResponderAEH()
{
}

void EchoResponderAEH::handle(const std::string& p_message)
{
  MessageES::Ptr mES = std::dynamic_pointer_cast<MessageES>(m_eventSource);

  try
  {
    mES->getSocket()->write(p_message);
  }
  catch (const std::exception& e)
  {
    m_hsha.remove(mES->getDescriptor());
  }
}
