#ifndef CS6300_CALL_STATEMENT_HPP
#define CS6300_CALL_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"
#include "AST/SymbolTable.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class Call : public Statement
{
public:
  Call(std::string n,
       int l,
       std::vector<std::shared_ptr<Expression>> args,
       std::shared_ptr<SymbolTable> symbols)
    : Statement(), _name(n), label(l), arguments(args), symbolTable(symbols)
  {
  }
  cs6300::FlowGraph emit();
  std::string ClassName() const;
  std::string name() const;

protected:
  std::vector<std::string> _ASTLines() const;

private:
  int label;
  std::string _name;
  std::vector<std::shared_ptr<Expression>> arguments;
  std::shared_ptr<SymbolTable> symbolTable;
};
}
#endif
