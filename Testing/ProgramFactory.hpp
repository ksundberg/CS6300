#ifndef __Program_Factory_H_
#define __Program_Factory_H_

#include <string>

#include "AST/SymbolTable.hpp"
#include "AST/Statements/Statement.hpp"

#include "AST/Statements/For.hpp"

class ProgramFactory
{
public:
  ProgramFactory();
  ProgramFactory& id(std::string name);
  ProgramFactory& assign(std::string lhs, int rhs);
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

  std::vector<std::shared_ptr<cs6300::Statement>> stms;

  std::string str();

private:
  std::shared_ptr<cs6300::SymbolTable> table;
};

#endif //__Program_Factory_H_
