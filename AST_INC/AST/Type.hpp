#ifndef TYPE_HPP
#define TYPE_HPP

#include <map>
#include <memory>
#include <string>

namespace cs6300
{
class Type
{
  public:
  virtual int size() = 0;
  virtual ~Type()=default;
};

class IntType:public Type {public: int size(){return 4;} };
class CharType:public Type {public: int size(){return 4;} };
class BoolType:public Type {public: int size(){return 4;} };
class StringType:public Type {public: int size(){return 0;} };

class RecordType : public Type
{
  public:
    RecordType()
        : Type()
        , fields()
    {
    }
  int size();
  std::map<std::string,std::shared_ptr<Type>> fields;
};

class ArrayType : public Type
{
  public:
    ArrayType(int lb, int ub, std::shared_ptr<Type> t)
        : Type()
        , baseType(t)
        , lowerbound(lb)
        , upperbound(ub)
    {
    }
  int size();
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
}
#endif

