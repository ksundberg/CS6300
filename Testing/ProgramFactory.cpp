#include "ProgramFactory.hpp"

#include "BackEnd/BackEnd.hpp"

#include "AST/Statements/Assignment.hpp"
#include "AST/Statements/Call.hpp"

ProgramFactory::ProgramFactory()
  : table(std::make_shared<cs6300::SymbolTable>(nullptr, cs6300::GLOBAL))
{
  cs6300::Expression::resetLabelCount();
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
  const std::string& name,
  int label,
  const std::vector<std::shared_ptr<cs6300::Expression>>& args)
{
  stms.emplace_back(std::make_shared<cs6300::Call>(name, label, args, table));
  return *this;
}

ProgramFactory& ProgramFactory::call(const std::string& name,
                                     int label,
                                     const std::vector<std::string>& args)
{
  std::vector<std::shared_ptr<cs6300::Expression>> expressions;
  for (auto&& id : args)
  {
    auto lval = std::make_shared<cs6300::LoadExpression>(
      std::make_shared<cs6300::IdAccess>(id, table));
    lval->setRef();
    expressions.push_back(lval);
  }

  stms.emplace_back(
    std::make_shared<cs6300::Call>(name, label, expressions, table));
  return *this;
}

ProgramFactory& ProgramFactory::callexpr(
  const std::string& name,
  const std::string& id,
  int label,
  std::vector<std::shared_ptr<cs6300::Expression>> args)
{
  exprs[name] = std::make_shared<cs6300::CallExpression>(
    id, label, args, cs6300::BuiltInType::getInt(), table);
  return *this;
}

ProgramFactory& ProgramFactory::for_(
  const std::string& var,
  const std::string& start,
  const std::string& end,
  cs6300::ForStatement::Direction d,
  const std::vector<std::shared_ptr<cs6300::Statement>>& statementList)
{
  stms.emplace_back(std::make_shared<cs6300::ForStatement>(
    var, exprs[start], exprs[end], d, statementList, table));
  return *this;
}

ProgramFactory& ProgramFactory::if_(
  const std::vector<
    std::pair<std::string, std::vector<std::shared_ptr<cs6300::Statement>>>>&
    strClauses,
  std::vector<std::shared_ptr<cs6300::Statement>> elseClause)
{
  std::vector<cs6300::IfStatement::clause_t> clauses;
  for (const auto& clause : strClauses)
    clauses.push_back({exprs[clause.first], clause.second});

  stms.emplace_back(std::make_shared<cs6300::IfStatement>(clauses, elseClause));
  return *this;
}

ProgramFactory& ProgramFactory::repeat(
  const std::string& name,
  std::vector<std::shared_ptr<cs6300::Statement>> innerStms)
{
  stms.emplace_back(
    std::make_shared<cs6300::RepeatStatement>(exprs[name], innerStms));
  return *this;
}

ProgramFactory& ProgramFactory::while_(
  const std::string& name,
  std::vector<std::shared_ptr<cs6300::Statement>> innerStms)
{
  stms.emplace_back(
    std::make_shared<cs6300::WhileStatement>(exprs[name], innerStms));
  return *this;
}

ProgramFactory& ProgramFactory::stop()
{
  stms.emplace_back(std::make_shared<cs6300::StopStatement>());
  return *this;
}

ProgramFactory& ProgramFactory::return_(const std::string& name)
{
  stms.emplace_back(std::make_shared<cs6300::ReturnStatement>(exprs[name]));
  return *this;
}

ProgramFactory& ProgramFactory::read(const std::vector<std::string>& ids)
{
  std::vector<std::shared_ptr<cs6300::LValue>> lvals;
  for (auto&& id : ids)
    lvals.push_back(std::make_shared<cs6300::IdAccess>(id, table));
  auto readstm = std::make_shared<cs6300::Read>(lvals[0]);
  for (int i = 1; i < lvals.size(); i++)
    readstm->append(lvals[i]);
  stms.emplace_back(readstm);
  return *this;
}

ProgramFactory& ProgramFactory::write(const std::vector<std::string>& strExprs)
{
  std::vector<std::shared_ptr<cs6300::Expression>> exprs_;
  for (auto&& expr : strExprs)
    exprs_.push_back(exprs[expr]);
  auto writestm = std::make_shared<cs6300::Write>(exprs_[0]);
  for (int i = 1; i < exprs_.size(); i++)
    writestm->append(exprs_[i]);
  stms.emplace_back(writestm);
  return *this;
}

ProgramFactory& ProgramFactory::load(const std::string& name,
                                     const std::string& id,
                                     bool ref)
{

  auto exp = std::make_shared<cs6300::LoadExpression>(
    std::make_shared<cs6300::IdAccess>(id, table));
  if (ref) exp->setRef();

  exprs[name] = exp;
  return *this;
}

ProgramFactory& ProgramFactory::memaccess(const std::string& name,
                                          int memory_value,
                                          int value_offset)
{
  exprs[name] = std::make_shared<cs6300::MemoryAccessExpression>(
    memory_value, value_offset);
  return *this;
}

ProgramFactory& ProgramFactory::literal(const std::string& name, int val)
{
  exprs[name] = std::make_shared<cs6300::LiteralExpression>(val);
  return *this;
}

ProgramFactory& ProgramFactory::literal(const std::string& name, char val)
{
  exprs[name] = std::make_shared<cs6300::LiteralExpression>(val);
  return *this;
}

ProgramFactory& ProgramFactory::literal(const std::string& name, bool val)
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

ProgramFactory& ProgramFactory::neg(const std::string& name,
                                    const std::string& exp)
{
  exprs[name] = std::make_shared<cs6300::UnaryMinusExpression>(exprs[exp]);
  return *this;
}

ProgramFactory& ProgramFactory::not_(const std::string& name,
                                     const std::string& exp)
{
  exprs[name] = std::make_shared<cs6300::NotExpression>(exprs[exp]);
  return *this;
}

ProgramFactory& ProgramFactory::pred(const std::string& name,
                                     const std::string& exp)
{
  exprs[name] = std::make_shared<cs6300::PredecessorExpression>(exprs[exp]);
  return *this;
}

ProgramFactory& ProgramFactory::succ(const std::string& name,
                                     const std::string& exp)
{
  exprs[name] = std::make_shared<cs6300::SuccessorExpression>(exprs[exp]);
  return *this;
}

ProgramFactory& ProgramFactory::string(const std::string& name,
                                       const std::string& exp)
{
  exprs[name] = std::make_shared<cs6300::StringExpression>(exp.c_str());
  return *this;
}

std::shared_ptr<cs6300::Statement> ProgramFactory::pop()
{
  auto ret = stms.back();
  stms.pop_back();
  return ret;
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

std::string ProgramFactory::exprstr(const std::string& name)
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

std::vector<std::shared_ptr<cs6300::Statement>> ProgramFactory::statements()
  const
{
  return stms;
}

void ProgramFactory::reset()
{
  table = std::make_shared<cs6300::SymbolTable>(nullptr, cs6300::GLOBAL);
  stms.clear();
  exprs.clear();
  cs6300::Expression::resetLabelCount();
  cs6300::BasicBlock::resetLabelCount();
}
