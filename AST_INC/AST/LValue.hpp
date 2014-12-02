#ifndef CS6300_LVALUE_HPP
#define CS6300_LVALUE_HPP

#include <string>
#include <memory>
#include "AST/Expressions/Expression.hpp"
#include "AST/Expressions/AdditionExpression.hpp"
#include "AST/Expressions/LiteralExpression.hpp"
#include "AST/Expressions/MultExpression.hpp"
#include "AST/Expressions/MemoryAccessExpression.hpp"
#include "AST/SymbolTable.hpp"
namespace cs6300
{
class LValue
{
public:
  LValue(std::shared_ptr<SymbolTable> t) : m_table(t) {}
  ~LValue() = default;
  virtual std::shared_ptr<Expression> address() const = 0;
  virtual std::shared_ptr<Type> type() const = 0;
  virtual bool isConst() const = 0;
  virtual std::shared_ptr<Expression> value() const = 0;

protected:
  std::shared_ptr<SymbolTable> m_table;
};

class IdAccess : public LValue
{
public:
  IdAccess(std::string n, std::shared_ptr<SymbolTable> t) : LValue(t), name(n)
  {
  }
  std::shared_ptr<Expression> address() const
  {
      auto entry =m_table->lookupVariable(name);
      auto location =entry->m_memorylocation;
      auto offset = entry->memory_offset;
    return std::make_shared<MemoryAccessExpression>(
      location,
     offset);
  }
  std::shared_ptr<Type> type() const
  {
    if (isConst())
      return m_table->lookupConstant(name)->type();
    return m_table->lookupVariable(name)->type;
  }
  bool isConst() const
  {
    return (m_table->lookupConstant(name) != nullptr);
  }
  std::shared_ptr<Expression> value() const
  {
    return m_table->lookupConstant(name);
  }
  std::string name;
};
class MemberAccess : public LValue
{
public:
  MemberAccess(std::shared_ptr<LValue> base,
               std::string field,
               std::shared_ptr<SymbolTable> t)
    : LValue(t), base(base), field(field)
  {
  }
  std::shared_ptr<Expression> address() const
  {
    int offset = 0;
    auto pSymbol = m_table->lookupRecordVariable(field, base->type());
    if(pSymbol)
      offset = pSymbol->memory_offset;

    return std::make_shared<MemoryAccessExpression>(
      pSymbol->m_memorylocation,
      offset);
  }
  std::shared_ptr<Type> type() const
  {
    return m_table->lookupType(field);
  }
  bool isConst() const
  {
    return false;
  }
  std::shared_ptr<Expression> value() const
  {
    return 0;
  }
  std::shared_ptr<LValue> base;
  std::string field;
};
class ArrayAccess : public LValue
{
public:
  ArrayAccess(std::shared_ptr<LValue> base,
              std::shared_ptr<Expression> e,
              std::shared_ptr<SymbolTable> t)
    : LValue(t), base(base), expr(e)
  {
  }
  std::shared_ptr<Expression> address() const
  {
    auto baseAddr = base->address();

    auto t = std::dynamic_pointer_cast<ArrayType>(base->type());
    auto size = std::make_shared<LiteralExpression>(t->baseType->size());
    auto offset = std::make_shared<MultExpression>(size, expr);
    return std::make_shared<AdditionExpression>(baseAddr, offset);
  }
  std::shared_ptr<Type> type() const
  {
    auto t = std::dynamic_pointer_cast<ArrayType>(base->type());
    return t->baseType;
  }
  bool isConst() const
  {
    return expr->isConst();
  }
  std::shared_ptr<Expression> value() const
  {
    return expr;
  }
  std::shared_ptr<LValue> base;
  std::shared_ptr<Expression> expr;
};
}

#endif

