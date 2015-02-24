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

ProgramFactory& ProgramFactory::assign(std::string lhs, std::string rhs)
{
  stms.emplace_back(std::make_shared<cs6300::Assignment>(
    std::make_shared<cs6300::IdAccess>(lhs, table), exprs[rhs]));
  return *this;
}

ProgramFactory& ProgramFactory::call(
  std::string name,
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

ProgramFactory& ProgramFactory::literal(std::string name, int val)
{
  exprs[name] = std::make_shared<cs6300::LiteralExpression>(val);
  return *this;
}

ProgramFactory& ProgramFactory::literal(std::string name, char val)
{
  exprs[name] = std::make_shared<cs6300::LiteralExpression>(val);
  return *this;
}

ProgramFactory& ProgramFactory::literal(std::string name, bool val)
{
  exprs[name] = std::make_shared<cs6300::LiteralExpression>(val);
  return *this;
}

#define BINARY_EXPR_DEF(function, class_name)                                  \
  BINARY_EXPR(ProgramFactory::function)                                        \
  {                                                                            \
    exprs[name] =                                                              \
      std::make_shared<cs6300::class_name>(exprs[lhs], exprs[rhs]);            \
    return *this;                                                              \
  }

BINARY_EXPR_DEF(add, AdditionExpression)
BINARY_EXPR_DEF(sub, SubtractExpression)
BINARY_EXPR_DEF(div, DivExpression)
BINARY_EXPR_DEF(mul, MultExpression)
BINARY_EXPR_DEF(and_, AndExpression)
BINARY_EXPR_DEF(or_, OrExpression)
BINARY_EXPR_DEF(eq, EqExpression)
BINARY_EXPR_DEF(neq, NeqExpression)
BINARY_EXPR_DEF(gte, GteExpression)
BINARY_EXPR_DEF(gt, GtExpression)
BINARY_EXPR_DEF(lte, LteExpression)
BINARY_EXPR_DEF(lt, LtExpression)
BINARY_EXPR_DEF(mod, ModExpression)

ProgramFactory& ProgramFactory::neg(std::string name, std::string exp)
{
  exprs[name] = std::make_shared<cs6300::UnaryMinusExpression>(exprs[exp]);
  return *this;
}

std::string ProgramFactory::str()
{
  std::ostringstream fout;
  cs6300::emitMIPS(cs6300::emitList(stms), fout);
  reset();
  auto s = fout.str();
  auto remove_tabs = [](char ch)
  {
    return ch == '\t';
  };
  s.erase(std::remove_if(s.begin(), s.end(), remove_tabs), s.end());
  return s;
}

std::string ProgramFactory::exprstr(std::string name)
{
  auto expr = exprs[name];
  auto bb = expr->emit();
  std::ostringstream fout;
  cs6300::emitMIPS(bb, fout);
  reset();
  auto s = fout.str();
  auto remove_tabs = [](char ch)
  {
    return ch == '\t';
  };
  s.erase(std::remove_if(s.begin(), s.end(), remove_tabs), s.end());
  return s;
}

std::vector<std::shared_ptr<cs6300::Expression>> ProgramFactory::expressions()
  const
{
  std::vector<std::shared_ptr<cs6300::Expression>> _exprs;
  for (auto exp : exprs)
    _exprs.push_back(exp.second);
  return _exprs;
}

void ProgramFactory::reset()
{
  table = std::make_shared<cs6300::SymbolTable>(nullptr, cs6300::GLOBAL);
  stms.clear();
  exprs.clear();
  cs6300::Expression::getNextLabel(true);
  cs6300::BasicBlock::getNextLabel(true);
}
