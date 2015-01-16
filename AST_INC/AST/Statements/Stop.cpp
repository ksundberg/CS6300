#include "Stop.hpp"

cs6300::FlowGraph cs6300::StopStatement::emit()
{
  auto block = std::make_shared<BasicBlock>();
  block->instructions.push_back(
    ThreeAddressInstruction(ThreeAddressInstruction::Stop, 0, 0, 0));
  return std::make_pair(block, block);
}
