#include "OrdExpression.hpp"

cs6300::OrdExpression::OrdExpression(std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::OrdExpression::emit() const
{
  return m_expr->emit();
}

std::shared_ptr<cs6300::Type> cs6300::OrdExpression::type() const
{
  return cs6300::BuiltInType::getInt();
}

int cs6300::OrdExpression::value() const
{
  return m_expr->value();
}

bool cs6300::OrdExpression::isConst() const
{
  return m_expr->isConst();
}

int cs6300::OrdExpression::getLabel() const
{
  return m_expr->getLabel();
}

std::string cs6300::OrdExpression::name() const
{
  return "Ord";
}

std::vector<std::string> cs6300::OrdExpression::ASTDot() const
{
  std::vector<std::string> lines;
  return join(m_expr, lines, id());
}
