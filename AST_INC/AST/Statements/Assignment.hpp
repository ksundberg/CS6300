#ifndef CS6300_ASSIGNMENT_STATEMENT_HPP
#define CS6300_ASSIGNMENT_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"
#include "AST/LValue.hpp"

namespace cs6300
{
class Assignment : public Statement
{
public:
  Assignment(std::shared_ptr<LValue> l, std::shared_ptr<Expression> e)
    : Statement(), lval(l), expr(e)
  {
  }

  cs6300::FlowGraph emit();

private:
  std::shared_ptr<LValue> lval;
  std::shared_ptr<Expression> expr;
};
}
#endif
