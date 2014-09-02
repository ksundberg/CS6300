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
  ReturnStatement(std::shared_ptr<Expression> v)
      : Statement()
      , value(v)
  {
  }
  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> emit();
private:
  std::shared_ptr<Expression> value;
};
}
#endif
