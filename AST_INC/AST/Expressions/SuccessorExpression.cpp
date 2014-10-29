#include "SuccessorExpression.hpp"

cs6300::SuccessorExpression::SuccessorExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::SuccessorExpression::emit() const

{
  auto result = m_expr->emit();
  result->instructions.emplace_back(
    ThreeAddressInstruction::AddValue, getLabel(), m_expr->getLabel(), 1);
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::SuccessorExpression::type() const
{
  return nullptr;
}

int cs6300::SuccessorExpression::value() const
{
  return m_expr->value() - 1;
}
bool cs6300::SuccessorExpression::isConst() const
{
  return m_expr->isConst();
}
