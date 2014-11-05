#include "Return.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::ReturnStatement::emit()
{
  auto block = std::make_shared<BasicBlock>();

  if (value) {
    auto b = value->emit();
    std::copy(b->instructions.begin(),
              b->instructions.end(),
              std::back_inserter(block->instructions));

    block->instructions.push_back(ThreeAddressInstruction(
      ThreeAddressInstruction::Return, 0, value->getLabel(), 0));
  }
  else {
    block->instructions.push_back(ThreeAddressInstruction(
      ThreeAddressInstruction::Return, 0, 0, 0));
  }

  return std::make_pair(block, block);
}
