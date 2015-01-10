#ifndef CS6300_AST_AND_EXPRESSION_HPP
#define CS6300_AST_AND_EXPRESSION_HPP
#include "Expression.hpp"
namespace cs6300
{
class AndExpression : public Expression
{
public:
  AndExpression(std::shared_ptr<Expression>, std::shared_ptr<Expression>);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  int value() const;
  bool isConst() const;

private:
  std::shared_ptr<Expression> m_lhs;
  std::shared_ptr<Expression> m_rhs;
};
}
#endif
