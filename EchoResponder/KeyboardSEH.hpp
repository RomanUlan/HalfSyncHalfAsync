#ifndef KEYBOARD_EH_HPP
#define KEYBOARD_EH_HPP

#include "../EventEngines/SyncEventHandler.hpp"
#include "KeyboardES.hpp"
#include <fstream>

class KeyboardSEH : public SyncEventHandler<SocketES, std::string>
{
public:
  KeyboardSEH(const std::string&, KeyboardES::Ptr);
  virtual ~KeyboardSEH();

public:
  virtual std::string handle(const EventSource::EventTypes&);

private:
  std::ofstream m_file;
};

#endif
