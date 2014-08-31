#include <cstdio>
#include <memory>
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
#include "AST/Statements/Statement.hpp"
#include "AST/Type.hpp"

extern FILE* yyin;
extern int yyparse();

namespace
{
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
  std::shared_ptr<std::vector<std::string>> getList(int index)
  {
    return get(idLists, index);
  }
  int addList(std::shared_ptr<std::vector<std::string>> list)
  {
    return add(idLists, list);
  }
  void clearLists()
  {
    idLists.clear();
  };
  std::shared_ptr<cs6300::Type> getType(int index)
  {
    return get(types, index);
  }
  int addType(std::shared_ptr<cs6300::Type> type)
  {
    return add(types,type);
  }
  void clearTypes()
  {
    types.clear();
  }
  std::shared_ptr<cs6300::Expression> getExpression(int index)
  {
    return get(expressions,index);
  }
  int addExpression(std::shared_ptr<cs6300::Expression> expr)
  {
    return add(expressions,expr);
  }
  void clearExpressions()
  {
    expressions.clear();
  }
  std::vector<std::shared_ptr<cs6300::Expression>> getArguments(int)
  {
    return std::vector<std::shared_ptr<cs6300::Expression>>();
  }
  
  std::shared_ptr<cs6300::Statement> getStatement(int index)
  {
    return get(statements,index);
  }
  int addStatement(std::shared_ptr<cs6300::Statement> s)
  {
    return add(statements,s);
  }
  void clearStatements()
  {
    statements.clear();
  }

  std::shared_ptr<std::vector<std::shared_ptr<cs6300::Statement>>> getStatementList(int index)
  {
    return get(statementLists,index);
  }
  int addStatementList(
      std::shared_ptr<std::vector<std::shared_ptr<cs6300::Statement>>> list)
  {
    return add(statementLists,list);
  }

  std::shared_ptr<cs6300::LValue> getLValue(int index)
  {
    return get(lvals,index);
  }
  int addLValue(std::shared_ptr<cs6300::LValue> lval)
  {
    return add(lvals,lval);
  }

private:
  template <typename T>
  std::shared_ptr<T> get(std::vector<std::shared_ptr<T>> &src, int index)
  {
    if (index < static_cast<int>(src.size()))
    {
      return src[index];
    }
    return nullptr;
  }
  template <typename T>
  int add(std::vector<std::shared_ptr<T>> &src, std::shared_ptr<T> e)
  {
    auto index = src.size();
    src.push_back(e);
    return index;
  }
  static std::shared_ptr<FrontEndState> msInstance;
  std::vector<std::shared_ptr<cs6300::Expression> > expressions;
  std::vector<std::shared_ptr<cs6300::Type> > types;
  std::vector<std::shared_ptr<std::vector<std::string>>> idLists;
  std::vector<std::shared_ptr<std::vector<std::shared_ptr<cs6300::Statement>>>> statementLists;
  std::vector<std::shared_ptr<cs6300::Statement>> statements;
  std::vector<std::shared_ptr<cs6300::LValue>> lvals;
};

std::shared_ptr<FrontEndState> FrontEndState::msInstance;

template <typename ExprType> int binaryOp(int a, int b)
{
  auto state = FrontEndState::instance();
  auto A = state->getExpression(a);
  auto B = state->getExpression(b);
  return state->addExpression(std::make_shared<ExprType>(A, B));
}
template <typename ExprType> int unaryOp(int a)
{
  auto state = FrontEndState::instance();
  auto A = state->getExpression(a);
  return state->addExpression(std::make_shared<ExprType>(A));
}
}

std::shared_ptr<cs6300::Program> cs6300::parseCPSL(std::string filename) 
{
  FILE* inFile;
  inFile = fopen(filename.c_str(),"r");
  if(inFile == nullptr)
  {
    throw std::runtime_error("Could not open " + filename);
  }
  yyin = inFile;
  yyparse();
  return std::shared_ptr<cs6300::Program>();
}

int cs6300::AddExpr(int a, int b) { return binaryOp<AdditionExpression>(a, b); }
int cs6300::AddField(int listIndex, int typeIndex) {
  auto state = FrontEndState::instance();
  auto type = state->getType(typeIndex);
  if (!type)
    return 0;
  auto list = state->getList(listIndex);
  auto newType = std::make_shared<cs6300::RecordType>();
  for (auto &&id : *list) {
    newType->fields[id] = type;
  }
  return state->addType(newType);
}
int cs6300::AndExpr(int a, int b)
{
  return binaryOp<AndExpression>(a, b);
}
int cs6300::AppendArgs(int, int) {return 0;}
int cs6300::AppendElseList(int /*list*/,int /*expr*/,int /*statement*/){return 0;}
int cs6300::BuildArrayType(int lowerIndex, int upperIndex, int typeIndex)
{
  auto state = FrontEndState::instance();
  auto type = state->getType(typeIndex);
  auto lb = state->getExpression(lowerIndex);
  auto ub = state->getExpression(upperIndex);
  int lower = -1;
  int upper = -1;
  if (!type)
    return -1;
  if (!lb)
    return -1;
  if (!ub)
    return -1;
  if(!lb->isConst()) return -1;
  if(!ub->isConst()) return -1;
  lower = lb->value();
  upper = ub->value();
  auto array = std::make_shared<cs6300::ArrayType>(lower,upper,type);
  return state->addType(array);
}

int cs6300::Assign(int /*lval*/,int /*expr*/){return 0;}
int cs6300::CallExpr(char *a, int b) {
  auto state = FrontEndState::instance();
  return state->addExpression(
      std::make_shared<cs6300::CallExpression>(a, state->getArguments(b)));
}
int cs6300::CallProc(char *, int ) {return 0;}
int cs6300::CharExpr(char a)
{
  auto state = FrontEndState::instance();
  return state->addExpression(std::make_shared<cs6300::LiteralExpression>(a));
}
int cs6300::ChrExpr(int a) {return unaryOp<ChrExpression>(a);}
int cs6300::DivExpr(int a, int b){return binaryOp<DivExpression>(a,b);}
int cs6300::DownTo(int /*expr*/){return 0;}
int cs6300::EmptyArgs() {return 0;}
int cs6300::EqExpr(int a, int b){return binaryOp<EqExpression>(a,b);}
int cs6300::FieldList(int typeIndex, int field)
{
  auto state = FrontEndState::instance();
  if (typeIndex == -1)
  {
    return state->addType(std::make_shared<cs6300::RecordType>());
  }
  else
  {
    auto type = std::dynamic_pointer_cast<cs6300::RecordType>(
        state->getType(typeIndex));
    if (!type)
      return 0;
    auto fields =
        std::dynamic_pointer_cast<cs6300::RecordType>(state->getType(field));
    if (!fields)
      return 0;
    for (auto &&f : fields->fields)
    {
      type->fields.insert(f);
    }
    return typeIndex;
  }
}
int cs6300::For(int /*head*/,int /*to*/,int /*statement*/){return 0;}
int cs6300::ForHead(char* /*ident*/,int /*expr*/){return 0;}
int cs6300::GtExpr(int a, int b) {return binaryOp<GtExpression>(a,b);}
int cs6300::GteExpr(int a, int b) {return binaryOp<GteExpression>(a,b);}
int cs6300::IdentList(int listIndex, char *ident)
{
  auto state = FrontEndState::instance();
  std::shared_ptr<std::vector<std::string>> list;
  if (listIndex == -1)
  {
    list = std::make_shared<std::vector<std::string>>();
  }
  else
  {
    auto list = state->getList(listIndex);
  }
  if (!list)
    return 0;
  list->emplace_back(ident);
  delete (ident);
  if (listIndex == -1)
  {
    return state->addList(list);
  }
  else
  {
    return listIndex;
  }
}
int cs6300::If(int /*expr*/,int /*statement*/,int /*list*/, int /*elseStatement*/){return 0;}
int cs6300::IntExpr(int a)
{
  auto state = FrontEndState::instance();
  return state->addExpression(std::make_shared<cs6300::LiteralExpression>(a));
}
int cs6300::LoadArray(int lval, int exprIndex)
{
  auto state = FrontEndState::instance();
  auto base = state->getLValue(lval);
  auto expr = state->getExpression(exprIndex);
  return state->addLValue(std::make_shared<cs6300::ArrayAccess>(base, expr));
}
int cs6300::LoadExpr(int lvalIndex)
{
  auto state = FrontEndState::instance();
  auto lval = state->getLValue(lvalIndex);
  auto expr = std::make_shared<cs6300::LoadExpression>(lval);
  return state->addExpression(expr);
}
int cs6300::LoadId(char *ident)
{
  auto lval = std::make_shared<cs6300::IdAccess>(ident);
  delete (ident);
  return FrontEndState::instance()->addLValue(lval);
}
int cs6300::LoadMember(int lvalIndex, char *ident)
{
  auto state = FrontEndState::instance();
  auto base = state->getLValue(lvalIndex);

  auto lval = std::make_shared<cs6300::MemberAccess>(base, ident);
  delete (ident);
  return FrontEndState::instance()->addLValue(lval);
}
int cs6300::LookupType(char *) {return 0;}
int cs6300::LtExpr(int a, int b) {return binaryOp<LtExpression>(a,b);}
int cs6300::LteExpr(int a, int b) {return binaryOp<LteExpression>(a,b);}
int cs6300::ModExpr(int a, int b) {return binaryOp<ModExpression>(a,b);}
int cs6300::MultExpr(int a, int b) {return binaryOp<MultExpression>(a,b);}
int cs6300::NeqExpr(int a, int b) {return binaryOp<NeqExpression>(a,b);}
int cs6300::NewArgs(int) {return 0;}
int cs6300::NotExpr(int a) {return unaryOp<NotExpression>(a);}
int cs6300::OrExpr(int a, int b) {return binaryOp<OrExpression>(a,b);}
int cs6300::OrdExpr(int a) {return unaryOp<OrdExpression>(a);}
int cs6300::Parameter(int /*list*/,int /*type*/){return 0;}
int cs6300::ParameterList(int /*list*/,int /*parameter*/){return 0;}
int cs6300::PredExpr(int a) {return unaryOp<PredecessorExpression>(a);}
int cs6300::ReadValue(int /*lval*/){return 0;}
int cs6300::ReadValue(int /*statement*/,int /*lval*/){return 0;}
int cs6300::Repeat(int /*statement*/,int /*expr*/){return 0;}
int cs6300::Return(){return 0;}
int cs6300::Return(int /*expr*/){return 0;}
int cs6300::Signature(char*, int /*params */){return 0;}
int cs6300::Signature(char*, int /*params*/, int /*type*/){return 0;}
int cs6300::StatementList(int listIndex, int statementIndex)
{
  auto state = FrontEndState::instance();
  auto statement = state->getStatement(statementIndex);
  std::shared_ptr<std::vector<std::shared_ptr<cs6300::Statement>>> list;
  if (listIndex == -1)
  {
    list = std::make_shared<std::vector<std::shared_ptr<cs6300::Statement>>>();
  }
  else
  {
    list = state->getStatementList(listIndex);
  }
  state->clearExpressions();
  list->push_back(statement);
  if (listIndex == -1)
  {
    return state->addStatementList(list);
  }
  else
  {
    return listIndex;
  }
}
int cs6300::Stop(){return 0;}
int cs6300::StrExpr(char *a)
{
  auto state = FrontEndState::instance();
  return state->addExpression(std::make_shared<cs6300::StringExpression>(a));
}
int cs6300::SubExpr(int a, int b) {return binaryOp<SubtractExpression>(a,b);}
int cs6300::SuccExpr(int a) {return unaryOp<SuccessorExpression>(a);}
int cs6300::To(int /*expr*/){return 0;}
int cs6300::UnMinusExpr(int a) {return unaryOp<UnaryMinusExpression>(a);}
int cs6300::While(int /*expr*/, int /*statement*/){return 0;}
int cs6300::WriteExpr(int /*expr*/){return 0;}
int cs6300::WriteExpr(int /*statment*/,int /*expr*/){return 0;}
void cs6300::AddConstant(char *, int) {}
void cs6300::AddFunction(int /*signature*/){}
void cs6300::AddFunction(int /*signature*/,int /*body*/){}
void cs6300::AddProcedure(int /*signature*/){}
void cs6300::AddProcedure(int /*signature*/,int /*body*/){}
void cs6300::AddType(char *, int) {}
void cs6300::AddVariables(int /*list*/,int /*type*/){}
