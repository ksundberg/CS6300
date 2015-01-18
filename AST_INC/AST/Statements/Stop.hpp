#ifndef CS6300_STOP_STATEMENT_HPP
#define CS6300_STOP_STATEMENT_HPP

#include "Statement.hpp"

namespace cs6300
{
class StopStatement : public Statement
{
public:
  StopStatement() : Statement() {}
  cs6300::FlowGraph emit();
  std::string ClassName() const;

protected:
  std::vector<std::string> _ASTLines() const;
};
}
#endif
