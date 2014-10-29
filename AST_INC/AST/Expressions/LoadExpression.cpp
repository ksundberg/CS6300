#include "LoadExpression.hpp"
#include "AST/LValue.hpp"

cs6300::LoadExpression::LoadExpression(std::shared_ptr<cs6300::LValue> lval)
  : Expression(), lval(lval)
{
}
std::shared_ptr<cs6300::BasicBlock> cs6300::LoadExpression::emit() const
{
  std::shared_ptr<cs6300::BasicBlock> block;
  if (isConst()) {
      block = std::make_shared<cs6300::BasicBlock>();
      block->instructions.emplace_back(
         ThreeAddressInstruction::LoadValue, getLabel(), value(), 0);
      return block;
  }
  auto address = lval->address();
  block = address->emit();
  block->instructions.emplace_back(
    ThreeAddressInstruction::LoadMemory, getLabel(), address->getLabel(), 0);
  return block;
}
std::shared_ptr<cs6300::Type> cs6300::LoadExpression::type() const
{
  return lval->type();
}
int cs6300::LoadExpression::value() const
{
  if (isConst())
    return lval->value()->value();
  return 0;
}
bool cs6300::LoadExpression::isConst() const
{
  return lval->isConst();
}
