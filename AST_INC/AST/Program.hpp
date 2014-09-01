#ifndef CS6300_PROGRAM_HPP
#define CS6300_PROGRAM_HPP

#include <map>
#include <vector>
#include "SymbolTable.hpp"
#include "Function.hpp"
#include "Type.hpp"
namespace cs6300
{
class Program
{
  public:
    Program()
        : symbols(std::make_shared<SymbolTable>(Program::getPredefinedTable()))
        , functions()
    {
    }
    static std::shared_ptr<SymbolTable> getPredefinedTable()
    {
      auto table = std::make_shared<SymbolTable>(nullptr);
      table->addType("integer",BuiltInType::getInt());
      table->addType("INTEGER",BuiltInType::getInt());
      table->addType("boolean",BuiltInType::getBool());
      table->addType("BOOLEAN",BuiltInType::getBool());
      table->addType("char",BuiltInType::getChar());
      table->addType("CHAR",BuiltInType::getChar());
      table->addType("string",BuiltInType::getStr());
      table->addType("STRING",BuiltInType::getStr());
      return table;
    }
  std::shared_ptr<SymbolTable> symbols;
  std::map<cs6300::FunctionSignature, std::shared_ptr<Function>> functions;
};
}

#endif
