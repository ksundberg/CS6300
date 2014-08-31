#include "PredecessorExpression.hpp"

cs6300::PredecessorExpression::PredecessorExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::PredecessorExpression::emit() const
{
  return nullptr;
}
std::shared_ptr<cs6300::Type> cs6300::PredecessorExpression::type() const
{
  return nullptr;
}
int cs6300::PredecessorExpression::value() const { return 0; }
bool cs6300::PredecessorExpression::isConst() const { return false; }

