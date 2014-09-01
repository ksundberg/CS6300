#ifndef CS6300_SYMBOL_TABLE_HPP
#define CS6300_SYMBOL_TABLE_HPP

#include <memory>
#include <map>

namespace cs6300
{
class Type;
class Expression;

class SymbolTable
{
public:
  SymbolTable(std::shared_ptr<SymbolTable> parent)
    : m_parent(parent)
  {
  }
  std::shared_ptr<Type> lookupType(std::string id);
  void addConstant(std::string id,std::shared_ptr<Expression>);
  void addType(std::string id,std::shared_ptr<Type>);
  void addVariable(std::string id,std::shared_ptr<Type>);
  std::shared_ptr<SymbolTable> getParent()
  {
    return m_parent;
  }

private:
  std::shared_ptr<SymbolTable> m_parent;
  std::map<std::string,std::shared_ptr<Type>> m_types;
  std::map<std::string,std::shared_ptr<Type>> m_variables;
  std::map<std::string,std::shared_ptr<Expression>> m_consts;
};
}
#endif
