#ifndef ECHO_RESPONDER_EH_HPP
#define ECHO_RESPONDER_EH_HPP

#include "../EventEngines/SyncEventHandler.hpp"
#include "MessageES.hpp"

class EchoResponderSEH: public SyncEventHandler<SocketES, std::string>
{
public:
	EchoResponderSEH(MessageES::Ptr/*, Reactor&*/);
	virtual ~EchoResponderSEH();

public:
	virtual std::string handle(const EventSource::EventTypes&);

private:
	//Reactor& m_reactor;
};

#endif
