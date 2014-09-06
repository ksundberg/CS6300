#include "UnaryMinusExpression.hpp"

cs6300::UnaryMinusExpression::UnaryMinusExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::UnaryMinusExpression::emit() const
{
  auto result = m_expr->emit();
  result->instructions.emplace_back(
      ThreeAddressInstruction::UnaryMinus, getLabel(), m_expr->getLabel(), 0);
  return result;
}
std::shared_ptr<cs6300::Type> cs6300::UnaryMinusExpression::type() const
{
  return nullptr;
}
int cs6300::UnaryMinusExpression::value() const { return - m_expr->value(); }

bool cs6300::UnaryMinusExpression::isConst() const { return m_expr->isConst(); }

