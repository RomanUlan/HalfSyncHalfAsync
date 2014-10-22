#include "ListenerES.hpp"
#include "../IO/Epoll.hpp"

ListenerES::ListenerES(TcpSocket::Ptr p_tcpSocket, int p_port)
  : SocketES(p_tcpSocket)
{
  m_eventTypes.insert(Epoll::EventType::Error);
  m_eventTypes.insert(Epoll::EventType::In);
  m_eventTypes.insert(Epoll::EventType::Hup);
  m_eventTypes.insert(Epoll::EventType::RdHup);

  p_tcpSocket->setNonBlocking();
  p_tcpSocket->bind(p_port);
  p_tcpSocket->listen();
}

ListenerES::~ListenerES()
{
}

MessageES::Ptr ListenerES::accept() const
{
  TcpSocket::Ptr tcpSocket = boost::dynamic_pointer_cast<TcpSocket>(m_socket);
  TcpSocket::Ptr accepted = tcpSocket->accept();
  return MessageES::Ptr(new MessageES(accepted));
}

