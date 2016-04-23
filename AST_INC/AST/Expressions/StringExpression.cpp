#include <algorithm>

#include "StringExpression.hpp"
#include "AST/StringTable.hpp"

cs6300::StringExpression::StringExpression(const char* a)
  : m_label(StringTable::instance()->add(std::string(a)))
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::StringExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  result->instructions.emplace_back(
    ThreeAddressInstruction::LoadString, getLabel(), m_label, 0);
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::StringExpression::type() const
{
  return cs6300::BuiltInType::getStr();
}

int cs6300::StringExpression::value() const
{
  return getLabel();
}

bool cs6300::StringExpression::isConst() const
{
  return false;
}
std::string cs6300::StringExpression::name() const
{
  std::string name = StringTable::instance()->lookup(m_label);
  name.erase(0, 1);                  // erase first quotation
  name.erase(name.size() - 1);       // erase last quotation
  return "\"\\\"" + name + "\\\"\""; // return properly escaped quotations
}

std::vector<std::string> cs6300::StringExpression::ASTDot() const
{
  return {};
}
