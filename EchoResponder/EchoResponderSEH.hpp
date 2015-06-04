#ifndef ECHO_RESPONDER_SEH_HPP
#define ECHO_RESPONDER_SEH_HPP

#include "BaseTypes.hpp"
#include "MessageES.hpp"

class EchoResponderSEH: public SEH
{
public:
  EchoResponderSEH(MessageES::Ptr, HSHA&);
  virtual ~EchoResponderSEH();

public:
  virtual std::string handle(const EventSource::EventTypes&);

private:
  HSHA& m_hsha;
};

#endif
