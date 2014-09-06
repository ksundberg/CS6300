#include "LiteralExpression.hpp"

cs6300::LiteralExpression::LiteralExpression(char a)
  : m_type(BuiltInType::getChar())
  , m_value(static_cast<int>(a))
{
}

cs6300::LiteralExpression::LiteralExpression(int a)
  : m_type(BuiltInType::getInt())
  , m_value(a)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::LiteralExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  result->instructions.emplace_back(
      ThreeAddressInstruction::LoadValue, getLabel(), m_value, 0);
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::LiteralExpression::type() const
{
  return m_type;
}

int cs6300::LiteralExpression::value() const { return m_value; }

bool cs6300::LiteralExpression::isConst() const { return true; }

