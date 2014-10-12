#include "MemoryExpression.hpp"

cs6300::MemoryExpression::MemoryExpression(int a)
    : m_type(BuiltInType::getInt())
    , m_value(a)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::MemoryExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  result->instructions.emplace_back(
      ThreeAddressInstruction::LoadOffset, getLabel(), m_value, 0);
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::MemoryExpression::type() const
{
  return m_type;
}


int cs6300::MemoryExpression::value() const { return m_value; }

bool cs6300::MemoryExpression::isConst() const { return false; }
