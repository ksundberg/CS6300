#ifndef TYPE_HPP
#define TYPE_HPP

#include <map>
#include <memory>
#include <string>

#include "Type.hpp"

namespace cs6300
{
class Type
{
public:
  virtual int size() = 0;
  virtual std::string name() = 0;
  virtual ~Type() = default;
};

class IntType : public Type
{
public:
  int size() { return 4; }
  std::string name() { return "int"; }
};
class CharType : public Type
{
public:
  int size() { return 4; }
  std::string name() { return "char"; }
};
class BoolType : public Type
{
public:
  int size() { return 4; }
  std::string name() { return "bool"; }
};
class StringType : public Type
{
public:
  int size() { return 4; }
  std::string name() { return "string"; }
};

class RecordType : public Type
{
public:
  RecordType() : Type(), fields(), field_offset(), _offset(0) {}
  int size();
  std::string name() { return "record"; }
  void addMember(std::string, std::shared_ptr<Type>);
  std::shared_ptr<cs6300::Type> type(std::string) const;
  int offset(std::string) const;
  std::map<std::string, std::shared_ptr<Type>> fields;
  std::map<std::string, int> field_offset;

private:
  int _offset;
};

class ArrayType : public Type
{
public:
  ArrayType(int lb, int ub, std::shared_ptr<Type> t)
    : Type(), baseType(t), lowerbound(lb), upperbound(ub)
  {
  }
  int size();
  std::string name() { return "array"; }
  std::shared_ptr<Type> baseType;
  int lowerbound;
  int upperbound;
};

class BuiltInType
{
public:
  static std::shared_ptr<Type> getChar()
  {
    if (!m_char) m_char = std::make_shared<CharType>();
    return m_char;
  }
  static std::shared_ptr<Type> getInt()
  {
    if (!m_int) m_int = std::make_shared<IntType>();
    return m_int;
  }
  static std::shared_ptr<Type> getBool()
  {
    if (!m_bool) m_bool = std::make_shared<BoolType>();
    return m_bool;
  }
  static std::shared_ptr<Type> getStr()
  {
    if (!m_str) m_str = std::make_shared<StringType>();
    return m_str;
  }

private:
  static std::shared_ptr<Type> m_char;
  static std::shared_ptr<Type> m_int;
  static std::shared_ptr<Type> m_bool;
  static std::shared_ptr<Type> m_str;
};

class ReferenceType : public Type
{
public:
  ReferenceType(std::shared_ptr<Type> t) : Type(), type(t) {}
  virtual int size() override final { return 4; }
  virtual std::string name() override final
  {
    return "reference " + type->name();
  }
  virtual ~ReferenceType() = default;
  const std::shared_ptr<Type> type;
};
}
#endif
