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
  Write(std::shared_ptr<Expression> v) : Statement(), values()
  {
    values.push_back(v);
  }
  void append(std::shared_ptr<Expression> v) { values.push_back(v); }
  cs6300::FlowGraph emit();
  std::string ClassName() const;

protected:
  std::vector<std::string> _ASTLines() const;

private:
  std::vector<std::shared_ptr<Expression>> values;
};
}
#endif
