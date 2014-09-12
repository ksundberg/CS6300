#include "LoadExpression.hpp"
#include "AST/LValue.hpp"

cs6300::LoadExpression::LoadExpression(std::shared_ptr<cs6300::LValue> lval)
    : Expression()
    , lval(lval)
{
}
std::shared_ptr<cs6300::BasicBlock> cs6300::LoadExpression::emit() const
{
  auto address = lval->address();
  auto block = address->emit();
  block->instructions.emplace_back(ThreeAddressInstruction::LoadMemory,getLabel(),address->getLabel(),0);
  return block;
}
std::shared_ptr<cs6300::Type> cs6300::LoadExpression::type() const
{
  return nullptr;
}
int cs6300::LoadExpression::value() const
{
  return 0;
}
bool cs6300::LoadExpression::isConst() const
{
  return false;
}

