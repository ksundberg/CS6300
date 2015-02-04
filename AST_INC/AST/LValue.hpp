#ifndef CS6300_LVALUE_HPP
#define CS6300_LVALUE_HPP

#include <string>
#include <memory>
#include "AST/Expressions/Expression.hpp"
#include "AST/Expressions/AdditionExpression.hpp"
#include "AST/Expressions/SubtractExpression.hpp"
#include "AST/Expressions/LiteralExpression.hpp"
#include "AST/Expressions/MultExpression.hpp"
#include "AST/Expressions/MemoryAccessExpression.hpp"
#include "AST/SymbolTable.hpp"
#include "logger.h"

namespace cs6300
{
class LValue
{
public:
  LValue(std::shared_ptr<SymbolTable> t) : m_table(t), _id(-1) {}
  ~LValue() = default;
  virtual std::shared_ptr<Expression> address() const = 0;
  virtual std::shared_ptr<Type> type() const = 0;
  virtual bool isConst() const = 0;
  virtual std::shared_ptr<Expression> value() const = 0;
  virtual std::string name() const = 0;
  virtual std::vector<std::string> ASTDot() const = 0;

  /* We need a unique id for duplicate nodes in the graph */
  std::string id() const
  {
    static int id_count = 0;
    if (_id == -1) _id = ++id_count;

    return "LValue" + std::to_string(_id);
  }
  std::string node() const { return id() + " [label=" + name() + "]"; }

protected:
  std::shared_ptr<SymbolTable> m_table;
  mutable int _id;
};

std::vector<std::string>& join(const std::shared_ptr<LValue>& lval,
                               std::vector<std::string>& dst,
                               std::string id);

std::vector<std::string>& join(
  const std::vector<std::shared_ptr<cs6300::LValue>>& lval,
  std::vector<std::string>& dst,
  std::string id);

class IdAccess : public LValue
{
public:
  IdAccess(std::string n, std::shared_ptr<SymbolTable> t) : LValue(t), _name(n)
  {
  }
  std::shared_ptr<Expression> address() const
  {
    auto entry = m_table->lookupVariable(_name);
    if (!entry) LOG(FATAL) << "Invalid variable lookup " << _name << std::endl;
    auto location = entry->m_memorylocation;
    auto offset = entry->memory_offset;

    if (location !=
        cs6300::GLOBAL) // on the stack memory starts on the negative side
      offset += entry->type->size();

    return std::make_shared<MemoryAccessExpression>(location, offset);
  }
  std::shared_ptr<Type> type() const
  {
    if (isConst()) return m_table->lookupConstant(_name)->type();
    auto v = m_table->lookupVariable(_name);
    if(!v)
        LOG(FATAL) << "Unable to lookup variable " << _name;

    return v->type;
  }
  bool isConst() const { return (m_table->lookupConstant(_name) != nullptr); }
  std::shared_ptr<Expression> value() const
  {
    return m_table->lookupConstant(_name);
  }

  std::string name() const { return "IdAccess"; }

  std::vector<std::string> ASTDot() const
  {
    std::string nameid = id() + _name; // unique id for the lookup name
    return {nameid + " [label=" + _name + "]", id() + " -> " + nameid};
  }

  std::string _name;
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
    if (pSymbol) offset = pSymbol->memory_offset;

    return std::make_shared<MemoryAccessExpression>(
      pSymbol->m_memorylocation, offset);
  }
  std::shared_ptr<Type> type() const { return m_table->lookupType(field); }
  bool isConst() const { return false; }
  std::shared_ptr<Expression> value() const { return 0; }
  std::string name() const { return "MemberAccess"; }
  std::vector<std::string> ASTDot() const
  {
    std::vector<std::string> lines;
    return join(base, lines, id());
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
    auto type = std::dynamic_pointer_cast<ArrayType>(base->type());
    if (!type) LOG(FATAL) << "Programming Error";

    if (expr->isConst())
    {
      int _offset = type->baseType->size() * (expr->value() - type->lowerbound);
      if (_offset)
      {
        auto offset = std::make_shared<LiteralExpression>(_offset);
        return std::make_shared<AdditionExpression>(baseAddr, offset);
      }
      else
      {
        return baseAddr;
      }
    }
    else
    {
      auto lower = std::make_shared<LiteralExpression>(type->lowerbound);
      auto idx = std::make_shared<SubtractExpression>(expr, lower);

      auto size = std::make_shared<LiteralExpression>(type->baseType->size());
      auto offset = std::make_shared<MultExpression>(size, idx);
      return std::make_shared<AdditionExpression>(baseAddr, offset);
    }
  }
  std::shared_ptr<Type> type() const
  {
    auto type = std::dynamic_pointer_cast<ArrayType>(base->type());
    return type->baseType;
  }
  bool isConst() const { return false; }
  std::shared_ptr<Expression> value() const { return expr; }
  std::string name() const { return "ArrayAccess"; }
  std::vector<std::string> ASTDot() const
  {
    std::vector<std::string> lines;
    join(base, lines, id());
    return join(expr, lines, id());
  }
  std::shared_ptr<LValue> base;
  std::shared_ptr<Expression> expr;
};
}

#endif
