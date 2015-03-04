#include "Write.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

cs6300::FlowGraph cs6300::Write::emit()
{
  auto block = std::make_shared<BasicBlock>();
  block->instructions.push_back(
    ThreeAddressInstruction("Begin Write", file, line));
  for (auto&& val : values)
  {
    auto b = val->emit();
    std::copy(b->instructions.begin(),
              b->instructions.end(),
              std::back_inserter(block->instructions));
    auto type = val->type();
    auto rtype = std::dynamic_pointer_cast<ReferenceType>(type);
    if (rtype) type = rtype->type;
    if (type == BuiltInType::getInt())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteInt, 0, val->getLabel(), 0));
    }
    else if (type == BuiltInType::getChar())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteChar, 0, val->getLabel(), 0));
    }
    else if (type == BuiltInType::getBool())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteBool, 0, val->getLabel(), 0));
    }
    else if (type == BuiltInType::getStr())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::WriteStr, 0, val->value(), 0));
    }
    else
    {
      if (type)
        LOG(FATAL) << "Unsupported print type " << type->name() << " "
                   << val->name();
      else
        LOG(FATAL) << "Unsupported print type NULL";
    }
  }

  block->instructions.push_back(ThreeAddressInstruction("End Write"));
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
