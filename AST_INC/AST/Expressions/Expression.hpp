#ifndef CS6300_AST_EXPRESSION_HPP
#define CS6300_AST_EXPRESSION_HPP

#include "AST/BasicBlock.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/Type.hpp"
#include <memory>
#include <vector>

namespace cs6300
{

class Expression
{
public:
  Expression() : label(-1)
  {
  }
  virtual std::shared_ptr<BasicBlock> emit() const = 0;
  virtual std::shared_ptr<Type> type() const = 0;
  virtual int value() const = 0;
  virtual bool isConst() const = 0;
  int getLabel()const;
private:
  static int getNextLabel();
  mutable int label;
};

std::shared_ptr<BasicBlock> emitBinaryOp(ThreeAddressInstruction::Type,
                                         int,
                                         std::shared_ptr<Expression>,
                                         std::shared_ptr<Expression>);
}
#endif
