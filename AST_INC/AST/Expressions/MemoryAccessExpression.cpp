#include "MemoryAccessExpression.hpp"
#include "LiteralExpression.hpp"

cs6300::MemoryAccessExpression::MemoryAccessExpression(int memory_value,
                                                       int offset)
  : m_type(nullptr)
  , m_value(memory_value)
  , m_offset(std::make_shared<cs6300::LiteralExpression>(offset))
{
}

cs6300::MemoryAccessExpression::MemoryAccessExpression(
  std::shared_ptr<Expression> memory_location)
  : m_type(nullptr), m_value(0), m_offset(memory_location)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::MemoryAccessExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  if (m_offset->isConst() && m_value != 0)
  {
    result->instructions.emplace_back(ThreeAddressInstruction::LoadMemoryOffset,
                                      getLabel(),
                                      m_value,
                                      m_offset->value());
  }
  else
  {
    auto code = m_offset->emit();
    std::copy(code->instructions.begin(),
              code->instructions.end(),
              std::back_inserter(result->instructions));
    result->instructions.emplace_back(
      ThreeAddressInstruction::LoadMemory, getLabel(), m_offset->getLabel(), 0);
  }
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::MemoryAccessExpression::type() const
{
  return m_type;
}

int cs6300::MemoryAccessExpression::value() const
{
  return m_value;
}

bool cs6300::MemoryAccessExpression::isConst() const
{
  return false;
}
std::string cs6300::MemoryAccessExpression::name() const
{
  return "Memory Access";
}

std::vector<std::string> cs6300::MemoryAccessExpression::ASTDot() const
{
  return {};
}
