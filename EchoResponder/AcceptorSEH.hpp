#ifndef ACCEPTOR_SEH_HPP
#define ACCEPTOR_SEH_HPP

#include "BaseTypes.hpp"
#include "ListenerES.hpp"

class AcceptorSEH: public SEH
{
public:
  AcceptorSEH(ListenerES::Ptr, HSHA&);
  virtual ~AcceptorSEH();

public:
  virtual std::string handle(const EventSource::EventTypes&);

private:
  HSHA& m_hsha;
};

#endif
