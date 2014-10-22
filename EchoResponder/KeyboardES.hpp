#ifndef KEYBOARD_ES_HPP
#define KEYBOARD_ES_HPP

#include "SocketES.hpp"
#include "../IO/KeyboardSocket.hpp"

class KeyboardES: public SocketES
{
public:
  typedef boost::shared_ptr<KeyboardES> Ptr;

public:
  explicit KeyboardES(KeyboardSocket::Ptr);
  virtual ~KeyboardES();

  void read(std::string&);
};

#endif
