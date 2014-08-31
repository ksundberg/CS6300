#ifndef CS6300_AST_EXPRESSION_HPP
#define CS6300_AST_EXPRESSION_HPP

#include <memory>
#include <vector>
#include "AST/BasicBlock.hpp"
#include "AST/Type.hpp"

namespace cs6300

{

class Expression
{
  public:
  virtual std::shared_ptr<BasicBlock> emit()const = 0;
  virtual std::shared_ptr<Type> type()const = 0;
  virtual int value()const=0;
  virtual bool isConst()const=0;
};
}
#endif
