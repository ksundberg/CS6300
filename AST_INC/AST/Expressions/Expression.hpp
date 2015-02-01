#ifndef CS6300_AST_EXPRESSION_HPP
#define CS6300_AST_EXPRESSION_HPP

#include "AST/BasicBlock.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/Type.hpp"
#include "AST/Statements/Statement.hpp"
#include <memory>
#include <vector>

namespace cs6300
{

class Expression
{
public:
  Expression() : label(-1) {}
  virtual std::shared_ptr<BasicBlock> emit() const = 0;
  virtual std::shared_ptr<Type> type() const = 0;
  virtual int value() const = 0;
  virtual bool isConst() const = 0;
  virtual std::string name() const = 0;
  virtual std::vector<std::string> ASTDot() const = 0;
  int getLabel() const;
  std::string id() const;
  virtual std::string node() const;
  static int getNextLabel();

private:
  mutable int label;
};

std::shared_ptr<BasicBlock> emitBinaryOp(ThreeAddressInstruction::Type,
                                         int,
                                         std::shared_ptr<Expression>,
                                         std::shared_ptr<Expression>);

std::vector<std::string>& join(const std::shared_ptr<cs6300::Expression>& expr,
                               std::vector<std::string>& dst,
                               std::string id,
                               std::string lbl = "");

std::vector<std::string>& join(
  const std::vector<std::shared_ptr<cs6300::Expression>>& exprs,
  std::vector<std::string>& dst,
  std::string id);
}
#endif
