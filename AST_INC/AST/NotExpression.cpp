#include "NotExpression.hpp"

cs6300::NotExpression::NotExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::NotExpression::emit() const
{
  return nullptr;
}
std::shared_ptr<cs6300::Type> cs6300::NotExpression::type() const
{
  return nullptr;
}
int cs6300::NotExpression::value() const { return 0; }
bool cs6300::NotExpression::isConst() const { return false; }

