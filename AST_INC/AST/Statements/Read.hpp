#ifndef CS6300_READ_STATEMENT_HPP
#define CS6300_READ_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/LValue.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class Read : public Statement
{
public:
  Read(std::shared_ptr<LValue> v)
      : Statement()
      , values()
  {
    values.push_back(v);
  }
  void append(std::shared_ptr<LValue> v)
  {
    values.push_back(v);
  }

  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> emit();

private:
  std::vector<std::shared_ptr<LValue>> values;
};
}
#endif
