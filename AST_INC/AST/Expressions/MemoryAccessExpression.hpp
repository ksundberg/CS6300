#ifndef CS6300_AST_MEMORY_EXPRESSION_HPP
#define CS6300_AST_MEMORY_EXPRESSION_HPP
#include "Expression.hpp"
#include <memory>

namespace cs6300
{
class MemoryAccessExpression : public Expression
{
public:
  MemoryAccessExpression(int memory_value, int offset);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  bool isConst() const;
  int value() const;
  int offset() const;

private:
  std::shared_ptr<Type> m_type;
  int m_value;
  int m_offset;
};
}
#endif
