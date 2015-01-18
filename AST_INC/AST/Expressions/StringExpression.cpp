#include <algorithm>

#include "StringExpression.hpp"
#include "AST/StringTable.hpp"

cs6300::StringExpression::StringExpression(char* a)
  : m_label(StringTable::instance()->add(std::string(a)))
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::StringExpression::emit() const
{
  return std::make_shared<BasicBlock>();
}

std::shared_ptr<cs6300::Type> cs6300::StringExpression::type() const
{
  return cs6300::BuiltInType::getStr();
}

int cs6300::StringExpression::value() const
{
  return m_label;
}

bool cs6300::StringExpression::isConst() const
{
  return true;
}
std::string cs6300::StringExpression::name() const
{
  std::string name = StringTable::instance()->lookup(m_label);
  name.erase(0,1); //erase first quotation
  name.erase(name.size()-1); //erase last quotation
  return "\"\\\"" + name + "\\\"\""; //return properly escaped quotations
}

std::vector<std::string> cs6300::StringExpression::ASTDot() const
{
  return {};
}
