#include "LtExpression.hpp"
cs6300::LtExpression::LtExpression (std::shared_ptr<Expression> lhs,
                                               std::shared_ptr<Expression> rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::LtExpression::emit() const
{
  return emitBinaryOp(
      ThreeAddressInstruction::IsLess, getLabel(), m_lhs, m_rhs);
}

std::shared_ptr<cs6300::Type> cs6300::LtExpression::type() const
{
  if (!m_lhs) return nullptr;
  if (!m_rhs) return nullptr;
  if (m_lhs->type() != m_rhs->type()) return nullptr;
  return m_lhs->type();
}

int cs6300::LtExpression::value() const
{
  if (!isConst()) return 0;
  return m_lhs->value() < m_rhs->value();
}
bool cs6300::LtExpression ::isConst() const
{
  if (!m_lhs) return false;
  if (!m_rhs) return false;
  return m_lhs->isConst() && m_rhs->isConst();
}

