#ifndef TYPE_HPP
#define TYPE_HPP

class Type
{
  public:
  virtual int size() = 0;
};

class IntType:public Type {public: int size(){return 4;} };
class CharType:public Type {public: int size(){return 4;} };
class BoolType:public Type {public: int size(){return 4;} };
class StringType:public Type {public: int size(){return 0;} };

#endif

