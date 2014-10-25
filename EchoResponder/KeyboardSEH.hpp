#ifndef KEYBOARD_SEH_HPP
#define KEYBOARD_SEH_HPP

#include "BaseTypes.hpp"
#include "KeyboardES.hpp"
#include <fstream>

class KeyboardSEH : public SEH
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
