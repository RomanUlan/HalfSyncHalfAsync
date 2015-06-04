#ifndef ECHO_RESPONDER_AEH_HPP
#define ECHO_RESPONDER_AEH_HPP

#include "BaseTypes.hpp"
#include "MessageES.hpp"

class EchoResponderAEH: public AEH
{
public:
  EchoResponderAEH(MessageES::Ptr, HSHA&);
  virtual ~EchoResponderAEH();

public:
  virtual void handle(const std::string&);

private:
  HSHA& m_hsha;
};

#endif //ECHO_RESPONDER_AEH_HPP
