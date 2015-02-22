#include "ProgramFactory.hpp"

#include "BackEnd/BackEnd.hpp"

#include "AST/Statements/Assignment.hpp"
#include "AST/Statements/Call.hpp"

ProgramFactory::ProgramFactory()
  : table(std::make_shared<cs6300::SymbolTable>(nullptr, cs6300::GLOBAL))
{
  cs6300::Expression::getNextLabel(true);
}
ProgramFactory& ProgramFactory::id(std::string name)
{
  table->addVariable(name, cs6300::BuiltInType::getInt());
  return *this;
}
ProgramFactory& ProgramFactory::assign(std::string lhs, int rhs)
{
  stms.emplace_back(std::make_shared<cs6300::Assignment>(
    std::make_shared<cs6300::IdAccess>(lhs, table),
    std::make_shared<cs6300::LiteralExpression>(rhs)));
  return *this;
}
ProgramFactory& ProgramFactory::call(std::string name,
                     int label,
                     std::vector<std::shared_ptr<cs6300::Expression>> args)
{
  stms.emplace_back(std::make_shared<cs6300::Call>(name, label, args, table));
  return *this;
}
ProgramFactory& ProgramFactory::for_(
  std::string var,
  int start,
  int end,
  cs6300::ForStatement::Direction d,
  std::vector<std::shared_ptr<cs6300::Statement>> statementList)
{
  stms.emplace_back(std::make_shared<cs6300::ForStatement>(
    var,
    std::make_shared<cs6300::LiteralExpression>(start),
    std::make_shared<cs6300::LiteralExpression>(end),
    d,
    statementList,
    table));
  return *this;
}

std::string ProgramFactory::str()
{
  std::ostringstream fout;
  cs6300::emitMIPS(cs6300::emitList(stms), fout);
  table = std::make_shared<cs6300::SymbolTable>(nullptr, cs6300::GLOBAL);
  stms.clear();
  cs6300::Expression::getNextLabel(true);
  auto s = fout.str();
  auto remove_tabs = [](char ch)
  {
    return ch == '\t';
  };
  s.erase(std::remove_if(s.begin(), s.end(), remove_tabs), s.end());
  return s;
}
