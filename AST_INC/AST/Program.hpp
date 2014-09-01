#ifndef CS6300_PROGRAM_HPP
#define CS6300_PROGRAM_HPP

#include <map>
#include <vector>
#include "SymbolTable.hpp"
#include "Function.hpp"
namespace cs6300
{
class Program
{
  public:
  std::shared_ptr<SymbolTable> symbols;
  std::map<cs6300::FunctionSignature, std::shared_ptr<Function>> functions;
};
}

#endif
