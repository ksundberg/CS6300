#include "Call.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/SymbolTable.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::Call::emit()
{
  auto block = std::make_shared<BasicBlock>();
    
  // Allocate space on the stack for parameters
  int spaceNeeded = 0;
    
  for (auto &&arg : arguments)
  {
    spaceNeeded += arg->type()->size();
  }

  
  if (spaceNeeded > 0) {
    block->instructions.push_back(ThreeAddressInstruction(ThreeAddressInstruction::AddValue, cs6300::MemoryLocation::STACK, cs6300::MemoryLocation::STACK, -spaceNeeded));
  }
    
    
//  block->instructions.push_back(ThreeAddressInstruction(
//
//
//    ThreeAddressInstruction::StoreFrame, 0, 0, 0));
    // TODO: set up arguments
    
    
    int loc = 0;
    for (auto&& arg = arguments.cbegin(); arg != arguments.cend(); arg++) {
        std::shared_ptr<BasicBlock> b = (*arg)->emit();
         block->instructions.insert(block->instructions.end(), b->instructions.begin(), b->instructions.end());
        
        loc += (*arg)->type()->size();
        block->instructions.push_back(ThreeAddressInstruction(ThreeAddressInstruction::StoreParameter, (*arg)->getLabel(), 0, loc));

    }

  block->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::CallFunction, 0, label, 0)); // set fp and jump

  // TODO: move sp back according to arguments size
    if (spaceNeeded > 0) {
        block->instructions.push_back(ThreeAddressInstruction(ThreeAddressInstruction::AddValue, cs6300::MemoryLocation::STACK, cs6300::MemoryLocation::STACK, spaceNeeded));

    }

//  block->instructions.push_back(ThreeAddressInstruction(
//    ThreeAddressInstruction::RestoreFrame, 0, 0, 0));

  return std::make_pair(block, block);
}
