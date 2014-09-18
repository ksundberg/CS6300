#ifndef CS6300_FUNCTION_HPP
#define CS6300_FUNCTION_HPP

#include <memory>
#include <string>
#include "SymbolTable.hpp"
#include "BasicBlock.hpp"

#include "AST/Type.hpp"
#include "AST/Statements/Statement.hpp"

namespace cs6300
{
class FunctionSignature
{
public:
  FunctionSignature(
    std::string n,
    std::vector<std::pair<std::string, std::shared_ptr<Type>>> a,
    std::shared_ptr<Type> t)
    : name(n), args(a), returnType(t)
  {
  }
  std::string name;
  std::vector<std::pair<std::string, std::shared_ptr<Type>>> args;
  std::shared_ptr<Type> returnType;
  bool operator==(const FunctionSignature&) const;
  bool operator<(const FunctionSignature&) const;
};

class Function
{
public:
  Function(std::shared_ptr<FunctionSignature> s,
           std::vector<std::shared_ptr<cs6300::Statement>> b,
           std::shared_ptr<SymbolTable> sym)
    : signature(s), body(b), symbols(sym)
  {
  }
  std::shared_ptr<FunctionSignature> signature;
  std::vector<std::shared_ptr<cs6300::Statement>> body;
  std::shared_ptr<SymbolTable> symbols;
};
}
#endif
