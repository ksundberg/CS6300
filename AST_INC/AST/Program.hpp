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
      table->addType(std::string("integer"),BuiltInType::getInt());
      table->addType(std::string("INTEGER"),BuiltInType::getInt());
      table->addType(std::string("boolean"),BuiltInType::getBool());
      table->addType(std::string("BOOLEAN"),BuiltInType::getBool());
      table->addType(std::string("char"),BuiltInType::getChar());
      table->addType(std::string("CHAR"),BuiltInType::getChar());
      table->addType(std::string("string"),BuiltInType::getStr());
      table->addType(std::string("STRING"),BuiltInType::getStr());
      return table;
    }
  std::shared_ptr<SymbolTable> symbols;
  std::map<cs6300::FunctionSignature, std::shared_ptr<Function>> functions;
  std::vector<std::shared_ptr<cs6300::Statement>> main;
};
}

#endif
