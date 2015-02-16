#include "LoadExpression.hpp"
#include "AST/LValue.hpp"

cs6300::LoadExpression::LoadExpression(std::shared_ptr<cs6300::LValue> lval)
  : Expression(), lval(lval)
{
}
std::shared_ptr<cs6300::BasicBlock> cs6300::LoadExpression::emit() const
{
  std::shared_ptr<cs6300::BasicBlock> block =
    std::make_shared<cs6300::BasicBlock>();
  block->instructions.emplace_back("Begin LoadExpression " + lval->name());
  if (isConst())
  {
    block->instructions.emplace_back(
      ThreeAddressInstruction::LoadValue, getLabel(), value(), 0);
    block->instructions.emplace_back("End (const) LoadExpression");
    return block;
  }

  auto address = lval->address();
  auto addrexpr = address->emit();

  block->instructions.emplace_back("begin addrexpr");
  std::copy(addrexpr->instructions.begin(),
            addrexpr->instructions.end(),
            std::back_inserter(block->instructions));
  block->instructions.emplace_back("end addrexpr");
  if (!(std::dynamic_pointer_cast<ArrayType>(lval->type()) ||
        std::dynamic_pointer_cast<RecordType>(lval->type())))
  {
    block->instructions.emplace_back(
      ThreeAddressInstruction::LoadMemory, getLabel(), address->getLabel(), 0);
  }
  else
  {
    block->instructions.emplace_back(
      ThreeAddressInstruction::LoadLabel, getLabel(), address->getLabel(), 0);
  }

  block->instructions.emplace_back("End LoadExpression");
  return block;
}
std::shared_ptr<cs6300::Type> cs6300::LoadExpression::type() const
{
  return lval->type();
}
int cs6300::LoadExpression::value() const
{
  if (isConst()) return lval->value()->value();
  return 0;
}
bool cs6300::LoadExpression::isConst() const
{
  return lval->isConst();
}

std::string cs6300::LoadExpression::name() const
{
  return "Load";
}

std::vector<std::string> cs6300::LoadExpression::ASTDot() const
{
  std::vector<std::string> lines;
  return join(lval, lines, id());
}
