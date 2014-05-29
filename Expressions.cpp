#include "Expressions.hpp"
#include "RegisterPool.hpp"
#include "ProcessLog.hpp"
#include "StringTable.hpp"
#include "SymbolTable.hpp"
#include <fstream>

extern std::ostream cpslout;

namespace
{
  bool isValidPair(MemoryLocation* a, Expr* b)
  {
    if(!a) return false;
    if(!b) return false;

    return a->type == b->type;
  }
  bool isValidPair(Expr* a, Expr* b)
  {
    if(!a) return false;
    if(!b) return false;

    return a->type == b->type;
  }

  Expr* binaryOp(std::string op,Expr*a,Expr*b)
  {
    if(!isValidPair(a,b))
    {
      ProcessLog::getInstance()->logError("Type mismatch in expression");
      delete(a);
      delete(b);
      return nullptr;
    }
    auto result = new Expr();
    result->reg = RegisterPool::getInstance()->allocate();
    result->type = a->type;
    cpslout << "\t" << op << " "
            << result->reg << ","
            << a->reg << ","
            << b->reg << std::endl;
    delete(a);
    delete(b);
    return result;
  }
  Expr* binaryOpWithMove(std::string op,std::string source,Expr*a,Expr*b)
  {
    if(!isValidPair(a,b))
    {
      ProcessLog::getInstance()->logError("Type mismatch in expression");
      delete(a);
      delete(b);
      return nullptr;
    }
    auto result = new Expr();
    result->reg = RegisterPool::getInstance()->allocate();
    result->type = a->type;
    cpslout << "\t" << op << " "
            << a->reg << ","
            << b->reg << std::endl;
    cpslout << "\tmf" << source
            << " " << result->reg << std::endl;
    delete(a);
    delete(b);
    return result;
  }
  Expr* unaryOp(std::string op,Expr*a)
  {
    if(!a)
    {
      ProcessLog::getInstance()->logError("Missing expression");
      return nullptr;
    }
    cpslout << "\t" << op << " "
            << a->reg << ","
            << a->reg << std::endl;
    return a;
  }
}

Expr * emitOr(Expr * a, Expr * b){return binaryOp("or",a,b);}
Expr * emitAnd(Expr * a, Expr * b){return binaryOp("and",a,b);}
Expr * emitEq(Expr * a, Expr * b){return binaryOp("seq",a,b);}
Expr * emitNeq(Expr * a, Expr * b){return emitNot(binaryOp("seq",a,b));}
Expr * emitLte(Expr * a, Expr * b){return emitNot(binaryOp("slt",b,a));}
Expr * emitGte(Expr * a, Expr * b){return emitNot(binaryOp("slt",a,b));}
Expr * emitLt(Expr * a, Expr * b){return binaryOp("slt",a,b);}
Expr * emitGt(Expr * a, Expr * b){return binaryOp("slt",b,a);}
Expr * emitAdd(Expr * a, Expr * b){return binaryOp("add",a,b);}
Expr * emitSub(Expr * a, Expr * b){return binaryOp("sub",a,b);}
Expr * emitMult(Expr * a, Expr * b){return binaryOpWithMove("mult","lo",a,b);}
Expr * emitDiv(Expr * a, Expr * b){return binaryOpWithMove("div","lo",a,b);}
Expr * emitMod(Expr * a, Expr * b){return binaryOpWithMove("div","hi",a,b);}
Expr * emitNot(Expr * a){return unaryOp("not",a);}
Expr * emitNeg(Expr * a){return unaryOp("neg",a);}

Expr * chr(Expr * a)
{
  if(a->type != SymbolTable::getInstance()->getIntType())
  {
    ProcessLog::getInstance()->logError("chr() only defined for integer expressions");
  }
  else
  {
    a->type = SymbolTable::getInstance()->getCharType();
  }
  return a;
}
Expr * ord(Expr * a)
{
  if(a->type != SymbolTable::getInstance()->getCharType())
  {
    ProcessLog::getInstance()->logError("chr() only defined for character expressions");
  }
  else
  {
    a->type = SymbolTable::getInstance()->getIntType();
  }
  return a;
}
Expr * pred(Expr * a)
{
  if(!a)
  {
    ProcessLog::getInstance()->logError("Missing expression");
    return nullptr;
  }
  if(a->type == SymbolTable::getInstance()->getBoolType())
  {
    return unaryOp("not",a);
  }
  else if((a->type == SymbolTable::getInstance()->getIntType())||(a->type == SymbolTable::getInstance()->getCharType()))
  {
    cpslout << "\taddi " << a->reg << ","
      << a->reg << ",1" << std::endl;
    return a;
  }
  else
  {
    ProcessLog::getInstance()->logError("pred() not defined for this expression");
  }
}
Expr * succ(Expr * a)
{
  if(!a)
  {
    ProcessLog::getInstance()->logError("Missing expression");
    return nullptr;
  }
  if(a->type == SymbolTable::getInstance()->getBoolType())
  {
    return unaryOp("not",a);
  }
  else if((a->type == SymbolTable::getInstance()->getIntType())||(a->type == SymbolTable::getInstance()->getCharType()))
  {
    cpslout << "\taddi " << a->reg << ","
      << a->reg << ",-1" << std::endl;
    return a;
  }
  else
  {
    ProcessLog::getInstance()->logError("succ() not defined for this expression");
  }
}
Expr * load(MemoryLocation * a)
{
  auto result = new Expr();
  if(!a)
  {
    ProcessLog::getInstance()->logError("Missing memory location");
  }
  else
  {
    result->reg = RegisterPool::getInstance()->allocate();
    result->type = a->type;
    cpslout << "\tlw "
      << result->reg << ","
      << a->offset << "(" << a->base << ")" << std::endl;
  }
  return result;
}
Expr * load(char * a)
{
  auto label = StringTable::getInstance()->add(a);
  auto result = new Expr();
  result->reg = RegisterPool::getInstance()->allocate();
  result->type = SymbolTable::getInstance()->getStringType();
  cpslout << "\tla "<< result->reg << "," << label << std::endl;
  return result;
}
Expr * load(char a)
{
  auto result = new Expr();
  result->reg = RegisterPool::getInstance()->allocate();
  result->type = SymbolTable::getInstance()->getCharType();
  cpslout << "\tli " << result->reg << "," << static_cast<int>(a) << std::endl;
  return result;
}
Expr * load(int a)
{
  auto result = new Expr();
  result->reg = RegisterPool::getInstance()->allocate();
  result->type = SymbolTable::getInstance()->getIntType();
  cpslout << "\tli " << result->reg << "," << a << std::endl;
  return result;
}
void store(MemoryLocation * loc, Expr * val)
{
  if(!isValidPair(loc,val))
  {
    ProcessLog::getInstance()->logError("Type mismatch in assignment");
  }
  else
  {
    cpslout << "\tsw " 
      << val->reg << ", " 
      << loc->offset << "(" << loc->base << ")" << std::endl;
  }
  delete(val);
}
void write(Expr * a)
{
  if(!a)
  {
    ProcessLog::getInstance()->logError("Missing expression");
    return;
  } 
  else if(a->type == SymbolTable::getInstance()->getCharType())
  {
    cpslout << "\tli $v0, 11" << std::endl;
    cpslout << "\tori $a0," << a->reg << ",0" << std::endl;
    cpslout << "\tsyscall" << std::endl;
  }
  else if(a->type == SymbolTable::getInstance()->getIntType())
  {
    cpslout << "\tli $v0, 1" << std::endl;
    cpslout << "\tori $a0," << a->reg << ",0" << std::endl;
    cpslout << "\tsyscall" << std::endl;
  }
  else if(a->type == SymbolTable::getInstance()->getStringType())
  {
    cpslout << "\tli $v0, 4" << std::endl;
    cpslout << "\tori $a0," << a->reg << ",0" << std::endl;
    cpslout << "\tsyscall" << std::endl;
  }
  else
  {
    ProcessLog::getInstance()->logError("write() only defined for basic types");
  }
  delete(a);
}
void read(MemoryLocation * loc)
{
  if(!loc)
  {
    ProcessLog::getInstance()->logError("Missing expression");
    return;
  } 
  else if(loc->type == SymbolTable::getInstance()->getCharType())
  {
    cpslout << "\tli $v0, 12" << std::endl;
    cpslout << "\tsyscall" << std::endl;
    cpslout << "\tsw $v0," << loc->offset
      << "(" << loc->base << ")" << std::endl;
  }
  else if(loc->type == SymbolTable::getInstance()->getIntType())
  {
    cpslout << "\tli $v0, 5" << std::endl;
    cpslout << "\tsyscall" << std::endl;
    cpslout << "\tsw $v0," << loc->offset
      << "(" << loc->base << ")" << std::endl;
  }
  else
  {
    ProcessLog::getInstance()->logError("read() only defined for basic types");
  }
}

