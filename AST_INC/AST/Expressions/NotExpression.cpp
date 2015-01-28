#include "NotExpression.hpp"

cs6300::NotExpression::NotExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::NotExpression::emit() const
{
  auto result = m_expr->emit();
  result->instructions.emplace_back(
      ThreeAddressInstruction::Not, getLabel(), m_expr->getLabel(), 0);
  return result;
}
std::shared_ptr<cs6300::Type> cs6300::NotExpression::type() const
{
  return m_expr->type();
}
int cs6300::NotExpression::value() const { return 0; }
bool cs6300::NotExpression::isConst() const { return false; }

std::string cs6300::NotExpression::name() const
{
  return "\"!\"";
}

std::vector<std::string> cs6300::NotExpression::ASTDot() const
{
  std::vector<std::string> lines;
  return join(m_expr, lines, id());
}
