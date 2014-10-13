#include "Epoll.hpp"

#include <stdexcept>
#include <sys/epoll.h>
#include <string.h>
#include <boost/scoped_array.hpp>

namespace
{
  uint32_t nativeEvents(const Epoll::EventTypes& p_ets)
  {
    uint32_t result = 0;
    for (Epoll::EventTypes::const_iterator i = p_ets.begin(); i != p_ets.end(); ++i)
    {
      if (*i == Epoll::EventType::Error)
        result |= EPOLLERR;
      else if (*i == Epoll::EventType::In)
        result |= EPOLLIN;
      else if (*i == Epoll::EventType::Out)
        result |= EPOLLOUT;
      else if (*i == Epoll::EventType::Hup)
        result |= EPOLLHUP;
      else if (*i == Epoll::EventType::RdHup)
        result |= EPOLLRDHUP;
      else
        throw std::runtime_error("Unknown event");
    }

    return result;
  }

  void nativeEvents(uint32_t p_nativeEvents, Epoll::EventTypes& p_ets)
  {
    if (p_nativeEvents & EPOLLERR)
      p_ets.insert(Epoll::EventType::Error);

    if (p_nativeEvents & EPOLLIN)
      p_ets.insert(Epoll::EventType::In);

    if (p_nativeEvents & EPOLLOUT)
      p_ets.insert(Epoll::EventType::Out);

    if (p_nativeEvents & EPOLLHUP)
          p_ets.insert(Epoll::EventType::Hup);

    if (p_nativeEvents & EPOLLRDHUP)
      p_ets.insert(Epoll::EventType::RdHup);
  }
}

int Epoll::EventType::Error = 0;
int Epoll::EventType::In = 1;
int Epoll::EventType::Out = 2;
int Epoll::EventType::Hup = 3;
int Epoll::EventType::RdHup = 4;

Epoll::Event::Event(const EventTypes& p_ets, Socket::Descriptor p_fd)
  : eventTypes(p_ets), descriptor(p_fd)
{
}

Epoll::Epoll(int p_size)
  : m_size(p_size), m_epollFd(::epoll_create(m_size))
{
  if (m_epollFd < 0)
  {
    throw std::runtime_error("Epoll create failed");
  }
}

Epoll::~Epoll()
{
  ::close(m_epollFd);
}

void Epoll::add(Socket::Ptr p_socket, const EventTypes& p_ets) const
{
  int fd = p_socket->getDescriptor();
  int option = EPOLL_CTL_ADD;

  epoll_event e;
  ::memset(&e, 0, sizeof(e));
  e.data.fd = fd;
  e.events = nativeEvents(p_ets);

  if (::epoll_ctl(m_epollFd, option, fd, &e) < 0)
  {
    throw std::runtime_error("Add handler to epoll failed");
  }
}

void Epoll::modify(Socket::Ptr, const EventTypes&) const
{
  //TODO
}

void Epoll::remove(Socket::Descriptor p_fd) const
{
  if (::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, p_fd, 0) < 0)
  {
    throw ::std::runtime_error("Cannot remove fd from epoll");
  }
}

void Epoll::wait(Events& p_events) const
{
  boost::scoped_array<epoll_event> events(new epoll_event[m_size]);
  ::memset(events.get(), 0, m_size * sizeof (epoll_event));
  int eventsLen = 0;

  eventsLen = ::epoll_wait(m_epollFd, events.get(), m_size, -1);
  if (eventsLen < 0)
  {
    throw ::std::runtime_error("Epoll wait failed");
  }

  for (int i = 0; i < eventsLen; ++i)
  {
    EventTypes ets;
    nativeEvents(events[i].events, ets);
    p_events.push_back(Event(ets, events[i].data.fd));
  }
}

