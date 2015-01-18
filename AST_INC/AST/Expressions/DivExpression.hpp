#ifndef CS6300_AST_DIV_EXPRESSION_HPP
#define CS6300_AST_DIV_EXPRESSION_HPP
#include "Expression.hpp"
namespace cs6300
{
class DivExpression : public Expression
{
public:
  DivExpression(std::shared_ptr<Expression>, std::shared_ptr<Expression>);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  int value() const;
  bool isConst() const;
  std::string name()const;
  std::vector<std::string> ASTDot()const;

private:
  std::shared_ptr<Expression> m_lhs;
  std::shared_ptr<Expression> m_rhs;
};
}
#endif
