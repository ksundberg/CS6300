#ifndef CS6300_AST_STRING_EXPRESSION_HPP
#define CS6300_AST_STRING_EXPRESSION_HPP
#include "Expression.hpp"
#include <string>
namespace cs6300
{
class StringExpression : public Expression
{
public:
  StringExpression(char*);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  int value() const;
  bool isConst() const;

private:
  int m_label;
};
}
#endif
