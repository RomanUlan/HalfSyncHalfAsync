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
    HSHA hsha(epollED, 1);

    KeyboardSocket::Ptr keybSocket(new KeyboardSocket());
    KeyboardES::Ptr keybES(new KeyboardES(keybSocket));
    SEH::Ptr keybSEH(new KeyboardSEH("log.txt", keybES));
    AEH::Ptr keybAEH;
    HSHA::Handlers keybHandlers(keybSEH, keybAEH);

    TcpSocket::Ptr listenerSokcet(new TcpSocket());
    ListenerES::Ptr listenerES(new ListenerES(listenerSokcet, 5050));
    SEH::Ptr acceptorSEH(new AcceptorSEH(listenerES, hsha));
    AEH::Ptr acceptorAEH;
    HSHA::Handlers acceptorHandlers(acceptorSEH, acceptorAEH);

    hsha.add(keybHandlers);
    hsha.add(acceptorHandlers);

    hsha.eventLoop();
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
