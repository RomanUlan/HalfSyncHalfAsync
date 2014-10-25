#include "../EventEngines/HalfSyncHalfAsync.hpp"
#include "../IO/Epoll.hpp"
#include "EpollED.hpp"
#include "AcceptorSEH.hpp"
#include "KeyboardSEH.hpp"

#include <iostream>
#include <stdexcept>

int main(int, char**)
{
	try
	{
		Epoll::Ptr epoll(new Epoll());
		EpollED::Ptr epollED(new EpollED(epoll));
		HalfSyncHalfAsync<EpollED, SocketES, std::string> hsha;

		/*KeyboardSocket::Ptr keybSocket(new KeyboardSocket());
		KeyboardES::Ptr keybES(new KeyboardES(keybSocket));
		SyncEventHandler::Ptr keybEH(new KeyboardEH("log.txt", keybES));

		TcpSocket::Ptr listenerSokcet(new TcpSocket());
		ListenerES::Ptr listenerES(new ListenerES(listenerSokcet, 5050));
		EventHandler::Ptr acceptorEH(new AcceptorEH(listenerES, reactor));

		reactor.add(keybEH);
		reactor.add(acceptorEH);

		reactor.eventLoop(); */
	}
	catch (const std::runtime_error& rte)
	{
		std::cout << "Runtime exception: " << rte.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "STD exception: " << e.what() << std::endl;
	}

	return 0;
}

