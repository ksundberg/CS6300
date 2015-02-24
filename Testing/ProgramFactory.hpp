#ifndef __Program_Factory_H_
#define __Program_Factory_H_

#include <string>

#include "AST/SymbolTable.hpp"
#include "AST/Statements/Statement.hpp"

#include "AST/Statements/For.hpp"

#include "AST/Expressions/AndExpression.hpp"
#include "AST/Expressions/ChrExpression.hpp"
#include "AST/Expressions/DivExpression.hpp"
#include "AST/Expressions/EqExpression.hpp"
#include "AST/Expressions/GteExpression.hpp"
#include "AST/Expressions/GtExpression.hpp"
#include "AST/Expressions/LteExpression.hpp"
#include "AST/Expressions/LtExpression.hpp"
#include "AST/Expressions/ModExpression.hpp"
#include "AST/Expressions/NeqExpression.hpp"
#include "AST/Expressions/OrExpression.hpp"
#include "AST/Expressions/OrdExpression.hpp"
#include "AST/Expressions/UnaryMinusExpression.hpp"

#define BINARY_EXPR(function)                                                  \
  ProgramFactory& function(std::string name, std::string lhs, std::string rhs)

class ProgramFactory
{
public:
  ProgramFactory();

  // Statements
  ProgramFactory& id(std::string name);
  ProgramFactory& assign(std::string lhs, std::string rhs);
  ProgramFactory& call(std::string name,
                       int label,
                       std::vector<std::shared_ptr<cs6300::Expression>> args =
                         std::vector<std::shared_ptr<cs6300::Expression>>());
  ProgramFactory& for_(
    std::string var,
    int start,
    int end,
    cs6300::ForStatement::Direction d,
    std::vector<std::shared_ptr<cs6300::Statement>> statementList);

  ProgramFactory& literal(std::string name, int val);
  ProgramFactory& literal(std::string name, char val);
  ProgramFactory& literal(std::string name, bool val);
  BINARY_EXPR(add);
  BINARY_EXPR(sub);
  BINARY_EXPR(div);
  BINARY_EXPR(mul);
  BINARY_EXPR(and_); // and is a keyword
  BINARY_EXPR(or_);  // or  is a keyword
  BINARY_EXPR(eq);
  BINARY_EXPR(neq);
  BINARY_EXPR(gte);
  BINARY_EXPR(gt);
  BINARY_EXPR(lte);
  BINARY_EXPR(lt);
  BINARY_EXPR(mod);

  ProgramFactory& neg(std::string name, std::string exp);

  std::string str();
  std::string exprstr(std::string name);
  std::vector<std::shared_ptr<cs6300::Expression>> expressions() const;
  void reset();

  std::vector<std::shared_ptr<cs6300::Statement>> stms;

private:
  std::shared_ptr<cs6300::SymbolTable> table;
  std::map<std::string, std::shared_ptr<cs6300::Expression>> exprs;
};

#endif //__Program_Factory_H_
