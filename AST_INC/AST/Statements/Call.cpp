#include "Call.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::Call::emit()
{
  auto block = std::make_shared<BasicBlock>();

  block->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::StoreFrame, 0, 0, 0));

  // TODO: set up arguments

  block->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::CallFunction, 0, label, 0)); // set fp and jump

  // TODO: move sp back according to arguments size

  block->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::RestoreFrame, 0, 0, 0));

  return std::make_pair(block, block);
}
