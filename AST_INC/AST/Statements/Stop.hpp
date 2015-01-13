#ifndef CS6300_STOP_STATEMENT_HPP
#define CS6300_STOP_STATEMENT_HPP

#include "Statement.hpp"

namespace cs6300
{
class StopStatement : public Statement
{
public:
  StopStatement() : Statement() {}
  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> emit();
};
}
#endif
