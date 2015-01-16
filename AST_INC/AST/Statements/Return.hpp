#ifndef CS6300_RETURN_STATEMENT_HPP
#define CS6300_RETURN_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class ReturnStatement : public Statement
{
public:
  ReturnStatement(std::shared_ptr<Expression> v) : Statement(), value(v) {}
  cs6300::FlowGraph emit();

private:
  std::shared_ptr<Expression> value;
};
}
#endif
