#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "Type.hpp"
#include <string>

namespace cs6300
{
class Symbol
{
public:
  int memory_offset = 0;
  std::string id;
  std::shared_ptr<Type> type;
  Symbol() = default;
  virtual ~Symbol() = default;
  Symbol(std::string& id, std::shared_ptr<Type> const& type, int memory_offset)
    : memory_offset(memory_offset), id(id), type(type)
  {
  }
};
}

#endif
