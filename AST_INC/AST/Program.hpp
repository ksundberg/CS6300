#ifndef CS6300_PROGRAM_HPP
#define CS6300_PROGRAM_HPP

#include <memory>
#include <vector>
#include "SymbolTable.hpp"
#include "Function.hpp"
namespace cs6300
{
class Program
{
  std::shared_ptr<SymbolTable> symbols;
  std::vector<std::shared_ptr<Function> > functions;
};
}

#endif
