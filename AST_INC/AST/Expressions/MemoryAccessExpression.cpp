#include "MemoryAccessExpression.hpp"

cs6300::MemoryAccessExpression::MemoryAccessExpression(int memory_value,
                                                       int offset)
  : m_type(nullptr), m_value(memory_value), m_offset(offset)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::MemoryAccessExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  result->instructions.emplace_back(
    ThreeAddressInstruction::LoadMemoryOffset, getLabel(), m_value, m_offset);
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::MemoryAccessExpression::type() const
{
  return m_type;
}

int cs6300::MemoryAccessExpression::offset() const
{
  return m_offset;
}

int cs6300::MemoryAccessExpression::value() const
{
  return m_value;
}

bool cs6300::MemoryAccessExpression::isConst() const
{
  return false;
}
