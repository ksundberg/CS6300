#ifndef CS6300_WRITE_STATEMENT_HPP
#define CS6300_WRITE_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class Write : public Statement
{
public:
  Write(std::shared_ptr<Expression> v)
      : Statement()
      , values()
  {
    values.push_back(v);
  }
  void append(std::shared_ptr<Expression> v)
  {
    values.push_back(v);
  }
  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> emit();
private:
  std::vector<std::shared_ptr<Expression>> values;
};
}
#endif
