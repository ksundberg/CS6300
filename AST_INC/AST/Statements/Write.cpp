#include "Write.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

cs6300::FlowGraph cs6300::Write::emit()
{
  auto block = std::make_shared<BasicBlock>();
  for (auto&& val : values)
  {
    auto b = val->emit();
    std::copy(b->instructions.begin(),
              b->instructions.end(),
              std::back_inserter(block->instructions));
    if (val->type() == BuiltInType::getInt())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteInt, 0, val->getLabel(), 0));
    }
    if (val->type() == BuiltInType::getChar())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteChar, 0, val->getLabel(), 0));
    }
    if (val->type() == BuiltInType::getBool())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteBool, 0, val->getLabel(), 0));
    }
    if (val->type() == BuiltInType::getStr())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteStr, 0, val->value(), 0));
    }
  }
  return std::make_pair(block, block);
}

std::string cs6300::Write::ClassName() const
{
  return "Write";
}

std::vector<std::string> cs6300::Write::_ASTLines() const
{
  std::vector<std::string> lines;
  return join(values, lines, id());
}
