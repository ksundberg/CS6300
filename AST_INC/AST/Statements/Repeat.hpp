#ifndef CS6300_REPEAT_STATEMENT_HPP
#define CS6300_REPEAT_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class RepeatStatement : public Statement
{
public:
  RepeatStatement(std::shared_ptr<Expression> c,
                  std::vector<std::shared_ptr<Statement>> statements)
    : Statement(), condition(c), body(statements)
  {
  }
  cs6300::FlowGraph emit();

private:
  std::shared_ptr<Expression> condition;
  std::vector<std::shared_ptr<Statement>> body;
};
}
#endif
