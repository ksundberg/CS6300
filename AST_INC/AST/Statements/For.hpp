#ifndef CS6300_FOR_STATEMENT_HPP
#define CS6300_FOR_STATEMENT_HPP

#include "Statement.hpp"
#include "AST/Expressions/Expression.hpp"
#include "AST/SymbolTable.hpp"

#include <string>
#include <vector>

namespace cs6300
{
class ForStatement : public Statement
  {
    public:
      enum Direction
      {
        TO,
        DOWNTO
      };
      ForStatement(std::string var,
                   std::shared_ptr<Expression> b,
                   std::shared_ptr<Expression> e,
                   Direction d,
                   std::vector<std::shared_ptr<Statement>> statements,
                   std::shared_ptr<SymbolTable> t)
          : Statement()
          , loopVariable(var)
          , begin(b)
          , end(e)
          , direction(d)
          , body(statements)
          , table(t)
      {
      }
    std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> emit();

    private:
      std::string loopVariable;
      std::shared_ptr<Expression> begin;
      std::shared_ptr<Expression> end;
      Direction direction;
      std::vector<std::shared_ptr<Statement>> body;
      std::shared_ptr<SymbolTable> table;
  };
}
#endif
