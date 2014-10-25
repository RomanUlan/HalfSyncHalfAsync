#ifndef ACCEPTOR_EH_HPP
#define ACCEPTOR_EH_HPP

#include "../EventEngines/SyncEventHandler.hpp"
#include "ListenerES.hpp"

class AcceptorSEH : public SyncEventHandler<SocketES, std::string>
{
public:
	AcceptorSEH(ListenerES::Ptr/*, Reactor&*/);
	virtual ~AcceptorSEH();

public:
	virtual std::string handle(const EventSource::EventTypes&);

private:
	//Reactor& m_reactor;
};

#endif
