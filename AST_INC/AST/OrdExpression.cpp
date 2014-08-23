#include "OrdExpression.hpp"

cs6300::OrdExpression::OrdExpression(std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::OrdExpression::emit() const
{
  return nullptr;
}

std::shared_ptr<cs6300::Type> cs6300::OrdExpression::type() const
{
  return cs6300::BuiltInType::getInt();
}

int cs6300::OrdExpression::value() const { return 0; }

bool cs6300::OrdExpression::isConst() const { return false; }
