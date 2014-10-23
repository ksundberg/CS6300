#include "Read.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::Read::emit()
{
  auto block = std::make_shared<BasicBlock>();
  for (auto&& val : values)
  {

    auto b = std::make_shared<LiteralExpression>(0);

    if (val->type() == BuiltInType::getInt())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::ReadInt, b->getLabel(), 0, 0));
    }
    if (val->type() == BuiltInType::getChar())
    {
      block->instructions.push_back(ThreeAddressInstruction(
        ThreeAddressInstruction::ReadChar, b->getLabel(), 0, 0));
    }

    auto c = val->address();
    auto d = c->emit();
    std::copy(d->instructions.begin(),
              d->instructions.end(),
              std::back_inserter(block->instructions));

    block->instructions.emplace_back(
      ThreeAddressInstruction::StoreMemory, b->getLabel(), c->getLabel(), 0);
  }

  return std::make_pair(block, block);
}
