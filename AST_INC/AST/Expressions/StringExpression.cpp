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
