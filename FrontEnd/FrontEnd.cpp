#include <cstdio>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <string>
#include "FrontEnd.hpp"

#include "AST/Expressions/AdditionExpression.hpp"
#include "AST/Expressions/AndExpression.hpp"
#include "AST/Expressions/CallExpression.hpp"
#include "AST/Expressions/ChrExpression.hpp"
#include "AST/Expressions/DivExpression.hpp"
#include "AST/Expressions/EqExpression.hpp"
#include "AST/Expressions/Expression.hpp"
#include "AST/Expressions/GtExpression.hpp"
#include "AST/Expressions/GteExpression.hpp"
#include "AST/Expressions/LiteralExpression.hpp"
#include "AST/Expressions/LoadExpression.hpp"
#include "AST/Expressions/LtExpression.hpp"
#include "AST/Expressions/LteExpression.hpp"
#include "AST/Expressions/ModExpression.hpp"
#include "AST/Expressions/MultExpression.hpp"
#include "AST/Expressions/NeqExpression.hpp"
#include "AST/Expressions/NotExpression.hpp"
#include "AST/Expressions/OrExpression.hpp"
#include "AST/Expressions/OrdExpression.hpp"
#include "AST/Expressions/PredecessorExpression.hpp"
#include "AST/Expressions/StringExpression.hpp"
#include "AST/Expressions/SubtractExpression.hpp"
#include "AST/Expressions/SuccessorExpression.hpp"
#include "AST/Expressions/UnaryMinusExpression.hpp"
#include "AST/LValue.hpp"
#include "AST/Statements/Assignment.hpp"
#include "AST/Statements/Call.hpp"
#include "AST/Statements/For.hpp"
#include "AST/Statements/If.hpp"
#include "AST/Statements/Read.hpp"
#include "AST/Statements/Repeat.hpp"
#include "AST/Statements/Return.hpp"
#include "AST/Statements/Statement.hpp"
#include "AST/Statements/Stop.hpp"
#include "AST/Statements/While.hpp"
#include "AST/Statements/Write.hpp"
#include "AST/Type.hpp"
#include "AST/Function.hpp"

extern FILE* yyin;
extern int yyparse();

namespace
{
template <typename T> class FEC
{
public:
  FEC() : src() {}
  std::shared_ptr<T> get(int index)
  {
    if (index < 0) return nullptr;
    if (index < static_cast<int>(src.size()) + 1)
    {
      return src[index];
    }
    return nullptr;
  }
  int add(std::shared_ptr<T> e)
  {
    auto index = src.size();
    src.push_back(e);
    return index;
  }
  void clear() { src.clear(); }

private:
  std::vector<std::shared_ptr<T>> src;
};

class FrontEndState
{
public:
  static std::shared_ptr<FrontEndState> instance()
  {
    if (!msInstance)
    {
      msInstance = std::make_shared<FrontEndState>();
    }
    return msInstance;
  }
  typedef std::vector<std::shared_ptr<cs6300::Statement>> slist_t;
  FEC<cs6300::Expression> expressions;
  FEC<cs6300::FunctionSignature> signatures;
  FEC<cs6300::Type> types;
  FEC<std::vector<std::string>> idLists;
  FEC<slist_t> statementLists;
  FEC<std::vector<std::shared_ptr<cs6300::Expression>>> actualArguments;
  FEC<std::vector<std::pair<std::string, std::shared_ptr<cs6300::Type>>>>
    formalArguments;
  FEC<cs6300::Statement> statements;
  FEC<cs6300::LValue> lvals;
  FEC<std::vector<std::pair<std::shared_ptr<cs6300::Expression>,
                            std::shared_ptr<slist_t>>>> clauses;
  FEC<std::pair<std::shared_ptr<cs6300::Expression>,
                cs6300::ForStatement::Direction>> toExprs;
  FEC<std::pair<std::string, std::shared_ptr<cs6300::Expression>>> forHeads;

  void initParse()
  {
    program = std::make_shared<cs6300::Program>();
    curSymTable = program->symbols;
  }
  std::shared_ptr<cs6300::Program> getProgram() { return program; }

  std::shared_ptr<cs6300::SymbolTable> getSymTable()
  {
    assert(curSymTable);
    return curSymTable;
  }
  void pushSymTable()
  {
    curSymTable =
      std::make_shared<cs6300::SymbolTable>(curSymTable, cs6300::STACK);
  }
  void popSymTable() { curSymTable = curSymTable->getParent(); }

private:
  static std::shared_ptr<FrontEndState> msInstance;
  std::shared_ptr<cs6300::Program> program;
  std::shared_ptr<cs6300::SymbolTable> curSymTable;
};

std::shared_ptr<FrontEndState> FrontEndState::msInstance;
template <typename T, typename U>
int appendList(FEC<T>& listSrc, int listIndex, U elem)
{
  if (listIndex == -1)
  {
    auto list = std::make_shared<T>();
    list->push_back(elem);
    return listSrc.add(list);
  }
  else
  {
    auto list = listSrc.get(listIndex);
    list->push_back(elem);
    return listIndex;
  }
}
template <typename T, typename U>
int appendList(FEC<T>& listSrc,
               int listIndex,
               FEC<U>& elementSrc,
               int elementIndex)
{
  auto elem = elementSrc.get(elementIndex);
  return appendList(listSrc, listIndex, elem);
}

template <typename ExprType> int binaryOp(int a, int b)
{
  auto state = FrontEndState::instance();
  auto A = state->expressions.get(a);
  auto B = state->expressions.get(b);
  return state->expressions.add(std::make_shared<ExprType>(A, B));
}
template <typename ExprType> int unaryOp(int a)
{
  auto state = FrontEndState::instance();
  auto A = state->expressions.get(a);
  return state->expressions.add(std::make_shared<ExprType>(A));
}
}

std::shared_ptr<cs6300::Program> cs6300::parseCPSL(std::string filename)
{
  FILE* inFile;
  inFile = fopen(filename.c_str(), "r");
  if (inFile == nullptr)
  {
    throw std::runtime_error("Could not open " + filename);
  }
  yyin = inFile;
  FrontEndState::instance()->initParse();
  yyparse();
  return FrontEndState::instance()->getProgram();
}

int cs6300::AddExpr(int a, int b)
{
  return binaryOp<AdditionExpression>(a, b);
}

int cs6300::AddField(int listIndex, int typeIndex)
{
  auto state = FrontEndState::instance();
  auto type = state->types.get(typeIndex);
  if (!type) return -1;
  auto list = state->idLists.get(listIndex);
  auto newType = std::make_shared<cs6300::RecordType>();
  for (auto&& id : *list)
  {
    newType->fields[id] = type;
    state->getSymTable()->addVariable(id, type);
  }
  return state->types.add(newType);
}

int cs6300::AndExpr(int a, int b)
{
  return binaryOp<AndExpression>(a, b);
}
int cs6300::ArgumentList(int listIndex, int exprIndex)
{
  auto state = FrontEndState::instance();
  return appendList(
    state->actualArguments, listIndex, state->expressions, exprIndex);
}
int cs6300::AppendElseList(int listIndex, int expr, int statement)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  auto list = state->statementLists.get(statement);
  return appendList(state->clauses, listIndex, std::make_pair(e, list));
}

int cs6300::BuildArrayType(int lowerIndex, int upperIndex, int typeIndex)
{
  auto state = FrontEndState::instance();
  auto type = state->types.get(typeIndex);
  auto lb = state->expressions.get(lowerIndex);
  auto ub = state->expressions.get(upperIndex);
  int lower = -1;
  int upper = -1;
  if (!type) return -1;
  if (!lb) return -1;
  if (!ub) return -1;
  if (!lb->isConst()) return -1;
  if (!ub->isConst()) return -1;
  lower = lb->value();
  upper = ub->value();
  auto array = std::make_shared<cs6300::ArrayType>(lower, upper, type);
  return state->types.add(array);
}

int cs6300::Assign(int lval, int expr)
{
  auto state = FrontEndState::instance();
  auto l = state->lvals.get(lval);
  auto e = state->expressions.get(expr);
  return state->statements.add(std::make_shared<cs6300::Assignment>(l, e));
}

int cs6300::CallExpr(char* a, int b)
{
  auto state = FrontEndState::instance();
  auto pArgs = state->actualArguments.get(b);
  std::vector<std::shared_ptr<cs6300::Expression>> args;
  if (pArgs)
  {
    std::copy(pArgs->begin(), pArgs->end(), std::back_inserter(args));
  }


  // build signature to get function label
  auto program = state->getProgram();
  auto sigArgs = std::vector<std::pair<std::string, std::shared_ptr<Type>>>();
  if (pArgs)
    for (int i = 0; i < pArgs->size(); i++)
      sigArgs.push_back(std::make_pair<std::string, std::shared_ptr<Type>>("", (*pArgs)[i]->type()));
  auto functionSig = FunctionSignature(a, sigArgs, nullptr);
  int label = -1;
  std::shared_ptr<Type> type = nullptr;
  for (auto iter : program->functions)
    if (iter.first == functionSig) { // find matching signature and get label
      label = iter.first.getLabel();
      type = iter.first.returnType;
      auto val = iter.second;
      program->functions.erase(iter.first); // we need label on function sig to be updated, so reinsert
      program->functions[iter.first] = val;
      break;
    }

  return state->expressions.add(
    std::make_shared<cs6300::CallExpression>(label, args, type));
}

int cs6300::CallProc(char* name, int argsIndex)
{
  auto state = FrontEndState::instance();
  auto a = state->actualArguments.get(argsIndex);
  std::vector<std::shared_ptr<cs6300::Expression>> args;
  if (a)
  {
    std::copy(a->begin(), a->end(), std::back_inserter(args));
  }

  // build signature to get function label
  auto program = state->getProgram();
  auto sigArgs = std::vector<std::pair<std::string, std::shared_ptr<Type>>>();
  if (a)
    for (int i = 0; i < a->size(); i++)
      sigArgs.push_back(std::make_pair<std::string, std::shared_ptr<Type>>("", (*a)[i]->type()));
  auto functionSig = FunctionSignature(name, sigArgs, nullptr);
  int label = -1;
  for (auto iter : program->functions)
    if (iter.first == functionSig) { // find matching signature and get label
      label = iter.first.getLabel();
      auto val = iter.second;
      program->functions.erase(iter.first); // we need label on function sig to be updated, so reinsert
      program->functions[iter.first] = val;
      break;
    }

  return state->statements.add(std::make_shared<cs6300::Call>(label, args));
}

int cs6300::CharExpr(char a)
{
  auto state = FrontEndState::instance();
  return state->expressions.add(std::make_shared<cs6300::LiteralExpression>(a));
}
int cs6300::ChrExpr(int a)
{
  return unaryOp<ChrExpression>(a);
}
int cs6300::DivExpr(int a, int b)
{
  return binaryOp<DivExpression>(a, b);
}
int cs6300::DownTo(int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  return state->toExprs.add(
    std::make_shared<std::pair<std::shared_ptr<cs6300::Expression>,
                               cs6300::ForStatement::Direction>>(
      e, cs6300::ForStatement::DOWNTO));
}
int cs6300::EqExpr(int a, int b)
{
  return binaryOp<EqExpression>(a, b);
}
int cs6300::FieldList(int typeIndex, int field)
{
  auto state = FrontEndState::instance();
  if (typeIndex == -1)
  {
    return state->types.add(std::make_shared<cs6300::RecordType>());
  }
  else
  {
    auto type = std::dynamic_pointer_cast<cs6300::RecordType>(
      state->types.get(typeIndex));
    if (!type) return -1;
    auto fields =
      std::dynamic_pointer_cast<cs6300::RecordType>(state->types.get(field));
    if (!fields) return -1;
    for (auto&& f : fields->fields)
    {
      type->fields.insert(f);
    }
    return typeIndex;
  }
}
int cs6300::For(int head, int to, int statement)
{
  auto state = FrontEndState::instance();
  auto h = state->forHeads.get(head);
  auto t = state->toExprs.get(to);
  auto s = state->statementLists.get(statement);
  auto f = std::make_shared<cs6300::ForStatement>(
    h->first, h->second, t->first, t->second, *s, state->getSymTable());
  state->getSymTable()->addVariable(h->first, t->first->type());
  return state->statements.add(f);
}

int cs6300::ForHead(char* ident, int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  auto head = std::make_shared<
    std::pair<std::string, std::shared_ptr<cs6300::Expression>>>(
    std::string(ident), e);
  delete (ident);
  return state->forHeads.add(head);
}

int cs6300::GtExpr(int a, int b)
{
  return binaryOp<GtExpression>(a, b);
}
int cs6300::GteExpr(int a, int b)
{
  return binaryOp<GteExpression>(a, b);
}
int cs6300::IdentList(int listIndex, char* ident)
{
  auto state = FrontEndState::instance();
  return appendList(state->idLists, listIndex, ident);
}
int cs6300::If(int expr, int statement, int list, int elseStatement)
{
  auto state = FrontEndState::instance();
  auto ifClause = std::make_pair(
    state->expressions.get(expr), *(state->statementLists.get(statement)));
  auto otherClauses = state->clauses.get(list);
  std::vector<cs6300::IfStatement::clause_t> allClauses;
  allClauses.push_back(ifClause);
  if (otherClauses)
  {
    for (auto&& c : *otherClauses)
    {
      if (c.second)
      {
        allClauses.push_back(std::make_pair(c.first, *(c.second)));
      }
    }
  }
  auto elseClause = state->statementLists.get(elseStatement);
  if (elseClause)
  {
    return state->statements.add(
      std::make_shared<cs6300::IfStatement>(allClauses, *elseClause));
  }
  else
  {
    return state->statements.add(std::make_shared<cs6300::IfStatement>(
      allClauses, std::vector<std::shared_ptr<Statement>>()));
  }
}

int cs6300::IntExpr(int a)
{
  auto state = FrontEndState::instance();
  return state->expressions.add(std::make_shared<cs6300::LiteralExpression>(a));
}
int cs6300::LoadArray(int lval, int exprIndex)
{
  auto state = FrontEndState::instance();
  auto base = state->lvals.get(lval);
  auto expr = state->expressions.get(exprIndex);
  return state->lvals.add(std::make_shared<cs6300::ArrayAccess>(
    base, expr, FrontEndState::instance()->getSymTable()));
}
int cs6300::LoadExpr(int lvalIndex)
{
  auto state = FrontEndState::instance();
  auto lval = state->lvals.get(lvalIndex);
  auto expr = std::make_shared<cs6300::LoadExpression>(lval);
  return state->expressions.add(expr);
}
int cs6300::LoadId(char* ident)
{
  auto lval = std::make_shared<cs6300::IdAccess>(
    ident, FrontEndState::instance()->getSymTable());
  delete (ident);
  return FrontEndState::instance()->lvals.add(lval);
}
int cs6300::LoadMember(int lvalIndex, char* ident)
{
  auto state = FrontEndState::instance();
  auto base = state->lvals.get(lvalIndex);

  auto lval = std::make_shared<cs6300::MemberAccess>(
    base, ident, FrontEndState::instance()->getSymTable());
  delete (ident);
  return FrontEndState::instance()->lvals.add(lval);
}
int cs6300::LookupType(char* ident)
{
  auto state = FrontEndState::instance();
  auto type = state->getSymTable()->lookupType(ident);
  delete (ident);
  return state->types.add(type);
}
int cs6300::LtExpr(int a, int b)
{
  return binaryOp<LtExpression>(a, b);
}
int cs6300::LteExpr(int a, int b)
{
  return binaryOp<LteExpression>(a, b);
}
int cs6300::ModExpr(int a, int b)
{
  return binaryOp<ModExpression>(a, b);
}
int cs6300::MultExpr(int a, int b)
{
  return binaryOp<MultExpression>(a, b);
}
int cs6300::NeqExpr(int a, int b)
{
  return binaryOp<NeqExpression>(a, b);
}
int cs6300::NotExpr(int a)
{
  return unaryOp<NotExpression>(a);
}
int cs6300::OrExpr(int a, int b)
{
  return binaryOp<OrExpression>(a, b);
}
int cs6300::OrdExpr(int a)
{
  return unaryOp<OrdExpression>(a);
}
int cs6300::Parameter(int list, int type)
{
  auto state = FrontEndState::instance();
  auto ids = state->idLists.get(list);
  auto t = state->types.get(type);
  auto p = std::make_shared<
    std::vector<std::pair<std::string, std::shared_ptr<cs6300::Type>>>>();
  for (auto&& id : *ids)
  {
    p->push_back(std::make_pair(id, t));
  }
  return state->formalArguments.add(p);
}
int cs6300::ParameterList(int list, int parameter)
{
  auto state = FrontEndState::instance();
  auto first = state->formalArguments.get(list);
  auto second = state->formalArguments.get(parameter);
  for (auto&& e : *second)
  {
    first->push_back(e);
  }
  return list;
}
int cs6300::PredExpr(int a)
{
  return unaryOp<PredecessorExpression>(a);
}
int cs6300::ReadValue(int lval)
{
  auto state = FrontEndState::instance();
  auto l = state->lvals.get(lval);
  return state->statements.add(std::make_shared<cs6300::Read>(l));
}
int cs6300::ReadValue(int statement, int lval)
{
  auto state = FrontEndState::instance();
  auto s =
    std::dynamic_pointer_cast<cs6300::Read>(state->statements.get(statement));
  auto l = state->lvals.get(lval);
  s->append(l);
  return statement;
}
int cs6300::Repeat(int statement, int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  auto s = state->statementLists.get(statement);
  return state->statements.add(
    std::make_shared<cs6300::RepeatStatement>(e, *s));
}
int cs6300::Return()
{
  return FrontEndState::instance()->statements.add(
    std::make_shared<cs6300::ReturnStatement>(nullptr));
}
int cs6300::Return(int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  return state->statements.add(std::make_shared<cs6300::ReturnStatement>(e));
}

int cs6300::Signature(char* ident, int params, int type)
{
  auto state = FrontEndState::instance();
  auto returnType = state->types.get(type); // May be nullptr
  auto parameters = state->formalArguments.get(params);
  std::vector<std::pair<std::string, std::shared_ptr<Type>>> a;
  if (parameters)
  {
    std::copy(parameters->begin(), parameters->end(), std::back_inserter(a));
  }
  auto sig = std::make_shared<cs6300::FunctionSignature>(ident, a, returnType);
  delete (ident);
  state->pushSymTable();
  return state->signatures.add(sig);
}
int cs6300::StatementList(int listIndex, int statementIndex)
{
  auto state = FrontEndState::instance();
  return appendList(
    state->statementLists, listIndex, state->statements, statementIndex);
}
int cs6300::Stop()
{
  return FrontEndState::instance()->statements.add(
    std::make_shared<cs6300::StopStatement>());
}
int cs6300::StrExpr(char* a)
{
  auto state = FrontEndState::instance();
  return state->expressions.add(std::make_shared<cs6300::StringExpression>(a));
}
int cs6300::SubExpr(int a, int b)
{
  return binaryOp<SubtractExpression>(a, b);
}
int cs6300::SuccExpr(int a)
{
  return unaryOp<SuccessorExpression>(a);
}
int cs6300::To(int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  return state->toExprs.add(
    std::make_shared<std::pair<std::shared_ptr<cs6300::Expression>,
                               cs6300::ForStatement::Direction>>(
      e, cs6300::ForStatement::TO));
}

int cs6300::UnMinusExpr(int a)
{
  return unaryOp<UnaryMinusExpression>(a);
}

int cs6300::While(int expr, int statement)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  auto s = state->statementLists.get(statement);
  return state->statements.add(std::make_shared<cs6300::WhileStatement>(e, *s));
}

int cs6300::WriteExpr(int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  return state->statements.add(std::make_shared<cs6300::Write>(e));
}

int cs6300::WriteExpr(int statement, int expr)
{
  auto state = FrontEndState::instance();
  auto s =
    std::dynamic_pointer_cast<cs6300::Write>(state->statements.get(statement));
  auto e = state->expressions.get(expr);
  s->append(e);
  return statement;
}

void cs6300::AddConstant(char* ident, int expr)
{
  auto state = FrontEndState::instance();
  auto e = state->expressions.get(expr);
  state->getSymTable()->addConstant(ident, e);
  delete (ident);
}
void cs6300::AddLiteral(std::string ident, int expr)
{
    auto state = FrontEndState::instance();
    auto e = state->expressions.get(expr);
    state->getSymTable()->addConstant(ident, e);
}
void cs6300::AddFunction(int signature)
{
  auto state = FrontEndState::instance();
  auto sig = state->signatures.get(signature);
  auto program = state->getProgram();
  auto iter = program->functions.find(*sig);
  if (iter == program->functions.end())
  {
    program->functions[*sig] = nullptr; // add forward declaration
  }
  state->popSymTable();
}
void cs6300::AddFunction(int signature, int body)
{
  auto state = FrontEndState::instance();
  auto sig = state->signatures.get(signature);
  auto b = state->statementLists.get(body);
  auto program = state->getProgram();
  program->functions[*sig] =
    std::make_shared<cs6300::Function>(sig, *b, state->getSymTable());
  state->popSymTable();
}

void cs6300::AddMain(int body)
{
  auto state = FrontEndState::instance();
  auto b = state->statementLists.get(body);
  b->push_back(std::make_shared<cs6300::StopStatement>());
  auto program = state->getProgram();
  std::copy(b->begin(), b->end(), std::back_inserter(program->main));

  int t = state->expressions.add(std::make_shared<cs6300::LiteralExpression>(1));
  int f = state->expressions.add(std::make_shared<cs6300::LiteralExpression>(0));
  AddLiteral("true",t);
  AddLiteral("TRUE",t);
  AddLiteral("false",f);
  AddLiteral("FALSE",f);
}
void cs6300::AddType(char* ident, int typeIndex)
{
  auto state = FrontEndState::instance();
  auto type = state->types.get(typeIndex);
  state->getSymTable()->addType(ident, type);
  delete (ident);
}
void cs6300::AddVariables(int list, int typeIndex)
{
  auto state = FrontEndState::instance();
  auto type = state->types.get(typeIndex);
  auto idList = state->idLists.get(list);
  for (auto&& id : *idList)
  {
    state->getSymTable()->addVariable(id, type);
  }
}
