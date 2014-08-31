#ifndef CS6300_AST_LOAD_EXPRESSION_HPP
#define CS6300_AST_LOAD_EXPRESSION_HPP
#include "Expression.hpp"
#include <memory>
namespace cs6300
{
class LValue;
class LoadExpression : public Expression
{
public:
  LoadExpression(std::shared_ptr<LValue> lval);
  std::shared_ptr<BasicBlock> emit() const;
  std::shared_ptr<Type> type() const;
  int value() const;
  bool isConst() const;

private:
  std::shared_ptr<LValue> lval;
};
}
#endif
