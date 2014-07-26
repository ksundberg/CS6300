#ifndef CS6300_SYMBOL_TABLE_HPP
#define CS6300_SYMBOL_TABLE_HPP

#include <memory>

namespace cs6300
{
class SymbolTable
{
public:
  SymbolTable(std::shared_ptr<SymbolTable> parent)
    : m_parent(parent)
  {
  }

private:
  std::shared_ptr<SymbolTable> m_parent;
};
}
#endif
