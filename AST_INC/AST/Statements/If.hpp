#ifndef CS6300_IF_STATEMENT_HPP
#define CS6300_IF_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class IfStatement : public Statement
{
public:
  typedef std::pair<std::shared_ptr<Expression>,
                    std::vector<std::shared_ptr<Statement>>> clause_t;
  IfStatement(std::vector<clause_t> c,
              std::vector<std::shared_ptr<Statement>> e)
    : Statement(), clauses(c), elseClause(e)
  {
  }
  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> emit();

private:
  std::vector<clause_t> clauses;
  std::vector<std::shared_ptr<Statement>> elseClause;
};
}
#endif
