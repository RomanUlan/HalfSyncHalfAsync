#ifndef ACCEPTOR_EH_HPP
#define ACCEPTOR_EH_HPP

#include "../EventEngines/EventHandler.hpp"
#include "ListenerES.hpp"

class Reactor;

class AcceptorEH : public EventHandler
{
public:
  AcceptorEH(ListenerES::Ptr, Reactor&);
  virtual ~AcceptorEH();

public:
  virtual void handle(const EventSource::EventTypes&);

private:
  Reactor& m_reactor;
};

#endif
