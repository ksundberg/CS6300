#include "PredecessorExpression.hpp"

cs6300::PredecessorExpression::PredecessorExpression(
  std::shared_ptr<Expression> expr)
  : m_expr(expr)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::PredecessorExpression::emit() const
{
  auto result = m_expr->emit();
  result->instructions.emplace_back(
    ThreeAddressInstruction::AddValue, getLabel(), m_expr->getLabel(), -1);
  return result;
}
std::shared_ptr<cs6300::Type> cs6300::PredecessorExpression::type() const
{
  return nullptr;
}
int cs6300::PredecessorExpression::value() const
{
  return m_expr->value() + 1;
}
bool cs6300::PredecessorExpression::isConst() const
{
  return m_expr->isConst();
}
std::string cs6300::PredecessorExpression::name() const
{
  return "Pred";
}

std::vector<std::string> cs6300::PredecessorExpression::ASTDot() const
{
  std::vector<std::string> lines;
  return join(m_expr, lines, id());
}
