#ifndef CS6300_LVALUE_HPP
#define CS6300_LVALUE_HPP

#include <string>
#include <memory>

#include "AST/Expressions/Expression.hpp"
namespace cs6300
{
class LValue
{
public:
  ~LValue() = default;
};

class IdAccess : public LValue
{
public:
  IdAccess(std::string n)
      : LValue()
      , name(n)
  {
  }
  std::string name;
};
class MemberAccess : public LValue
{
public:
  MemberAccess(std::shared_ptr<LValue> base, std::string field)
      : LValue()
      , base(base)
      , field(field)
  {
  }
  std::shared_ptr<LValue> base;
  std::string field;
};
class ArrayAccess : public LValue
{
public:
  ArrayAccess(std::shared_ptr<LValue> base, std::shared_ptr<Expression> e)
      : LValue()
      , base(base)
      , expr(e)
  {
  }
  std::shared_ptr<LValue> base;
  std::shared_ptr<Expression> expr;
};
}

#endif
