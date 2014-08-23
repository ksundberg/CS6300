#include <cstdio>
#include "FrontEnd.hpp"

#include "AST/AdditionExpression.hpp"
#include "AST/AndExpression.hpp"
#include "AST/BasicBlock.hpp"
#include "AST/CallExpression.hpp"
#include "AST/ChrExpression.hpp"
#include "AST/DivExpression.hpp"
#include "AST/EqExpression.hpp"
#include "AST/Expression.hpp"
#include "AST/GtExpression.hpp"
#include "AST/GteExpression.hpp"
#include "AST/LiteralExpression.hpp"
#include "AST/LtExpression.hpp"
#include "AST/LteExpression.hpp"
#include "AST/ModExpression.hpp"
#include "AST/MultExpression.hpp"
#include "AST/NeqExpression.hpp"
#include "AST/NotExpression.hpp"
#include "AST/OrExpression.hpp"
#include "AST/OrdExpression.hpp"
#include "AST/PredecessorExpression.hpp"
#include "AST/StringExpression.hpp"
#include "AST/SubtractExpression.hpp"
#include "AST/SuccessorExpression.hpp"
#include "AST/UnaryMinusExpression.hpp"

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
  std::vector<cs6300::BasicBlock> blocks;
  std::vector<cs6300::ThreeAddressInstruction> statements;
  std::shared_ptr<cs6300::Expression> getExpression(int index)
  {
    if(index < expressions.size())
    {
      return expressions[index];
    }
    return nullptr;
  }
  int addExpression(std::shared_ptr<cs6300::Expression> expr)
  {
    auto index = expressions.size();
    expressions.push_back(expr);
    return index;
  }
  void clearExpressions() { expressions.clear(); }
  std::vector<std::shared_ptr<cs6300::Expression>> getArguments(int)
  {
    return std::vector<std::shared_ptr<cs6300::Expression>>();
  }

private:
  static std::shared_ptr<FrontEndState> msInstance;
  std::vector<std::shared_ptr<cs6300::Expression> > expressions;
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
int cs6300::AddField(int list,int type){}
int cs6300::AndExpr(int a, int b) { return binaryOp<AndExpression>(a, b); }
int cs6300::AppendArgs(int, int) {}
int cs6300::AppendElseList(int list,int expr,int statement){}
int cs6300::ArrayType(int lower,int upper,int type){}
int cs6300::Assign(int lval,int expr){}
int cs6300::CallExpr(char *a, int b) {
  auto state = FrontEndState::instance();
  return state->addExpression(
    std::make_shared<cs6300::CallExpression>(a, state->getArguments(b)));
}
int cs6300::CallProc(char *a, int b) {}
int cs6300::CharExpr(char a)
{
  auto state = FrontEndState::instance();
  return state->addExpression(std::make_shared<cs6300::LiteralExpression>(a));
}
int cs6300::ChrExpr(int a) {return unaryOp<ChrExpression>(a);}
int cs6300::DivExpr(int a, int b){return binaryOp<DivExpression>(a,b);}
int cs6300::DownTo(int expr){}
int cs6300::EmptyArgs() {}
int cs6300::EqExpr(int a, int b){return binaryOp<EqExpression>(a,b);}
int cs6300::FieldList(int list,int field){}
int cs6300::For(int head,int to,int statement){}
int cs6300::ForHead(char* ident,int expr){}
int cs6300::GtExpr(int a, int b) {return binaryOp<GtExpression>(a,b);}
int cs6300::GteExpr(int a, int b) {return binaryOp<GteExpression>(a,b);}
int cs6300::IdentList(int list,char* ident){}
int cs6300::If(int expr,int statement,int list, int elseStatement){}
int cs6300::IntExpr(int a)
{
  auto state = FrontEndState::instance();
  return state->addExpression(std::make_shared<cs6300::LiteralExpression>(a));
}
int cs6300::LoadArray(int lval, int expr) {}
int cs6300::LoadId(char *ident) {}
int cs6300::LoadMember(int lval, char *ident) {}
int cs6300::LookupType(char *) {}
int cs6300::LtExpr(int a, int b) {return binaryOp<LtExpression>(a,b);}
int cs6300::LteExpr(int a, int b) {return binaryOp<LteExpression>(a,b);}
int cs6300::ModExpr(int a, int b) {return binaryOp<ModExpression>(a,b);}
int cs6300::MultExpr(int a, int b) {return binaryOp<MultExpression>(a,b);}
int cs6300::NeqExpr(int a, int b) {return binaryOp<NeqExpression>(a,b);}
int cs6300::NewArgs(int) {}
int cs6300::NotExpr(int a) {return unaryOp<NotExpression>(a);}
int cs6300::OrExpr(int a, int b) {return binaryOp<OrExpression>(a,b);}
int cs6300::OrdExpr(int a) {return unaryOp<OrdExpression>(a);}
int cs6300::Parameter(int list,int type){}
int cs6300::ParameterList(int list,int parameter){}
int cs6300::PredExpr(int a) {return unaryOp<PredecessorExpression>(a);}
int cs6300::ReadValue(int lval){}
int cs6300::ReadValue(int statement,int lval){}
int cs6300::RecordType(int fieldlist){}
int cs6300::Repeat(int statement,int expr){}
int cs6300::Return(){}
int cs6300::Return(int expr){}
int cs6300::Signature(char*, int params ){}
int cs6300::Signature(char*, int params, int type){}
int cs6300::StatementList(int list, int statement){FrontEndState::instance()->clearExpressions();}
int cs6300::Stop(){}
int cs6300::StrExpr(char *a)
{
  auto state = FrontEndState::instance();
  return state->addExpression(std::make_shared<cs6300::StringExpression>(a));
}
int cs6300::SubExpr(int a, int b) {return binaryOp<SubtractExpression>(a,b);}
int cs6300::SuccExpr(int a) {return unaryOp<SuccessorExpression>(a);}
int cs6300::To(int expr){}
int cs6300::UnMinusExpr(int a) {return unaryOp<UnaryMinusExpression>(a);}
int cs6300::While(int expr, int statement){}
int cs6300::WriteExpr(int expr){}
int cs6300::WriteExpr(int statment,int expr){}
void cs6300::AddConstant(char *, int) {}
void cs6300::AddFunction(int signature){}
void cs6300::AddFunction(int signature,int body){}
void cs6300::AddProcedure(int signature){}
void cs6300::AddProcedure(int signature,int body){}
void cs6300::AddType(char *, int) {}
void cs6300::AddVariables(int list,int type){}
