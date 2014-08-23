#include "SuccessorExpression.hpp"

cs6300::SuccessorExpression::SuccessorExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::SuccessorExpression::emit() const
{
  return nullptr;
}
std::shared_ptr<cs6300::Type> cs6300::SuccessorExpression::type() const
{
  return nullptr;
}
int cs6300::SuccessorExpression::value() const { return 0; }
bool cs6300::SuccessorExpression::isConst() const { return false; }

