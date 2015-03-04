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
  MemoryAccessExpression(std::shared_ptr<Expression> memory_location);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  bool isConst() const;
  std::string name() const;
  std::vector<std::string> ASTDot() const;
  int value() const;

private:
  std::shared_ptr<Type> m_type;
  int m_value;
  std::shared_ptr<Expression> m_offset;
};
}
#endif
