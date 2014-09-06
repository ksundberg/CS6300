#include "ChrExpression.hpp"
cs6300::ChrExpression::ChrExpression(std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::ChrExpression::emit() const
{
  // No code associated with Ord, it just changes type
  return std::make_shared<cs6300::BasicBlock>();
}

std::shared_ptr<cs6300::Type> cs6300::ChrExpression::type() const
{
  return BuiltInType::getChar();
}

int cs6300::ChrExpression::value() const { return 0; }

bool cs6300::ChrExpression::isConst() const { return false; }

