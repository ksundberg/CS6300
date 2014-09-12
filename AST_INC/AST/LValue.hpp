#ifndef CS6300_LVALUE_HPP
#define CS6300_LVALUE_HPP

#include <string>
#include <memory>

#include "AST/Expressions/Expression.hpp"
#include "AST/Expressions/AdditionExpression.hpp"
#include "AST/Expressions/LiteralExpression.hpp"
#include "AST/Expressions/MultExpression.hpp"
#include "AST/SymbolTable.hpp"
namespace cs6300
{
class LValue
{
public:
  LValue(std::shared_ptr<SymbolTable> t):m_table(t){}
  ~LValue() = default;
  virtual std::shared_ptr<Expression> address() const = 0;
  virtual std::shared_ptr<Type> type() const = 0;
protected:
  std::shared_ptr<SymbolTable> m_table;
};

class IdAccess : public LValue
{
public:
  IdAccess(std::string n,std::shared_ptr<SymbolTable> t)
      : LValue(t)
      , name(n)
  {
  }
  std::shared_ptr<Expression> address() const
  {
    return std::make_shared<LiteralExpression>(0);
  }
  std::shared_ptr<Type> type() const {return nullptr;}
  std::string name;
};
class MemberAccess : public LValue
{
public:
  MemberAccess(std::shared_ptr<LValue> base, std::string field,std::shared_ptr<SymbolTable> t)
      : LValue(t)
      , base(base)
      , field(field)
  {
  }
  std::shared_ptr<Expression> address() const
  {
    auto baseAddr = base->address();
    auto fieldOffset = std::make_shared<LiteralExpression>(0);
    return std::make_shared<AdditionExpression>(baseAddr,fieldOffset);
  }
  std::shared_ptr<Type> type() const {return nullptr;}
  std::shared_ptr<LValue> base;
  std::string field;
};
class ArrayAccess : public LValue
{
public:
  ArrayAccess(std::shared_ptr<LValue> base, std::shared_ptr<Expression> e,std::shared_ptr<SymbolTable> t)
      : LValue(t)
      , base(base)
      , expr(e)
  {
  }
  std::shared_ptr<Expression> address() const
  {
    auto baseAddr = base->address();
    auto size = std::make_shared<LiteralExpression>(base->type()->size());
    auto offset = std::make_shared<MultExpression>(size,expr);
    return std::make_shared<AdditionExpression>(baseAddr,offset);
  }
  std::shared_ptr<Type> type() const {return nullptr;}
  std::shared_ptr<LValue> base;
  std::shared_ptr<Expression> expr;
};
}

#endif
