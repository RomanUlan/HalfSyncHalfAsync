#include "KeyboardSEH.hpp"
#include "../IO/Epoll.hpp"

#include <stdexcept>
#include <boost/algorithm/string.hpp>

KeyboardSEH::KeyboardSEH(const std::string& p_fileName, KeyboardES::Ptr p_kES)
  : SyncEventHandler(p_kES), m_file(p_fileName.c_str(), std::ofstream::out)
{
  if (!m_file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }
}

KeyboardSEH::~KeyboardSEH()
{
  m_file.close();
}

std::string KeyboardSEH::handle(const EventSource::EventTypes& p_eventTypes)
{
  EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
  if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
  {
    KeyboardES::Ptr keyboardES = std::dynamic_pointer_cast<KeyboardES>(m_eventSource);
    std::string data;
    keyboardES->read(data);
    m_file << data;

    if (boost::istarts_with(data, "exit"))
    {
      throw std::runtime_error("exit");
    }
  }
  else
  {
    throw std::runtime_error("Bad event for for acceptor");
  }

  return std::string();
}
