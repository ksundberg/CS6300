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
  return m_expr->type();
}
int cs6300::UnaryMinusExpression::value() const { return - m_expr->value(); }

bool cs6300::UnaryMinusExpression::isConst() const { return m_expr->isConst(); }

std::string cs6300::UnaryMinusExpression::name() const
{
  return "Neg";
}

std::vector<std::string> cs6300::UnaryMinusExpression::ASTDot() const
{
  std::vector<std::string> lines;
  return join(m_expr, lines, id());
}
