#include "LteExpression.hpp"
cs6300::LteExpression::LteExpression (std::shared_ptr<Expression> lhs,
                                               std::shared_ptr<Expression> rhs)
  : m_lhs(lhs)
  , m_rhs(rhs)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::LteExpression::emit() const
{
  return emitBinaryOp(
      ThreeAddressInstruction::IsLessEqual, getLabel(), m_lhs, m_rhs);
}

std::shared_ptr<cs6300::Type> cs6300::LteExpression::type() const
{
  if (!m_lhs) return nullptr;
  if (!m_rhs) return nullptr;
  if (m_lhs->type() != m_rhs->type()) return nullptr;
  return m_lhs->type();
}

int cs6300::LteExpression::value() const
{
  if (!isConst()) return 0;
  return m_lhs->value() <= m_rhs->value();
}
bool cs6300::LteExpression ::isConst() const
{
  if (!m_lhs) return false;
  if (!m_rhs) return false;
  return m_lhs->isConst() && m_rhs->isConst();
}

