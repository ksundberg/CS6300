#include "Assignment.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

cs6300::FlowGraph cs6300::Assignment::emit()
{
  auto block = expr->emit();
  auto addr = lval->address();
  auto addrblock = addr->emit();
  std::copy(addrblock->instructions.begin(),
            addrblock->instructions.end(),
            std::back_inserter(block->instructions));
  block->instructions.emplace_back(ThreeAddressInstruction::StoreMemory,
                                   expr->getLabel(),
                                   addr->getLabel(),
                                   0);
  return std::make_pair(block, block);
}
