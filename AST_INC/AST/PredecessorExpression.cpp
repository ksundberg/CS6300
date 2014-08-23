#include "UnaryMinusExpression.hpp"

cs6300::UnaryMinusExpression::UnaryMinusExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::UnaryMinusExpression::emit() const
{
  return nullptr;
}
std::shared_ptr<cs6300::Type> cs6300::UnaryMinusExpression::type() const
{
  return nullptr;
}
int cs6300::UnaryMinusExpression::value() const { return 0; }
bool cs6300::UnaryMinusExpression::isConst() const { return false; }

