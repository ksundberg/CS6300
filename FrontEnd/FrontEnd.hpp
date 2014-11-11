#ifndef CS6300_FRONT_END_HPP
#define CS6300_FRONT_END_HPP

#include <memory>

#include "AST/Program.hpp"

namespace cs6300
{
/*Returns the Abstract Syntax Tree that is the result of parsing filename*/
std::shared_ptr<Program> parseCPSL(std::string filename);
/*Various functions called from the bison generated parser*/
int AddExpr(int a, int b);
int AddField(int list,int type);
int AndExpr(int a, int b);
int ArgumentList(int, int);
int AppendElseList(int list,int expr,int statement); 
int Assign(int lval,int expr);
int BuildArrayType(int lower,int upper,int type);
int CallExpr(char *a, int b);
int CallProc(char *a, int b);
int CharExpr(char a);
int ChrExpr(int a);
int DivExpr(int a, int b);
int DownTo(int expr);
int EmptyArgs();
int EqExpr(int a, int b);
int FieldList(int list,int field);
int For(int head,int to,int statement);
int ForHead(char* ident,int expr);
int GtExpr(int a, int b);
int GteExpr(int a, int b);
int IdentList(int list,char* ident);
int If(int expr,int statement,int list, int elseStatement);
int IntExpr(int a);
int LoadArray(int lval, int expr);
int LoadExpr(int lval);
int LoadId(char *ident);
int LoadMember(int lval, char *ident);
int LookupType(char *);
int LtExpr(int a, int b);
int LteExpr(int a, int b);
int ModExpr(int a, int b);
int MultExpr(int a, int b);
int NeqExpr(int a, int b);
int NotExpr(int a);
int OrExpr(int a, int b);
int OrdExpr(int a);
int Parameter(int list,int type);
int ParameterList(int list,int parameter);
int PredExpr(int a);
int ReadValue(int lval);
int ReadValue(int statement,int lval);
int Repeat(int statement,int expr);
int Return();
int Return(int expr);
int Signature(char*, int params, int type);
int StatementList(int list, int statement);
int Stop();
int StrExpr(char *a);
int SubExpr(int a, int b);
int SuccExpr(int a);
int To(int expr);
int UnMinusExpr(int a);
int While(int expr, int statement);
int WriteExpr(int expr);
int WriteExpr(int statment,int expr);
void AddConstant(char *, int);
void AddLiteral(std::string, int);
void AddFunction(int signature);
void AddFunction(int signature,int body);
void AddMain(int);
void AddType(char *, int);
void AddVariables(int list,int type);
}
#endif
