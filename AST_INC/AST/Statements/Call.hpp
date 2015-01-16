#ifndef CS6300_CALL_STATEMENT_HPP
#define CS6300_CALL_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class Call : public Statement
{
public:
  Call(int l, std::vector<std::shared_ptr<Expression>> args)
    : Statement(), label(l), arguments(args)
  {
  }
  cs6300::FlowGraph emit();

private:
  int label;
  std::vector<std::shared_ptr<Expression>> arguments;
};
}
#endif
