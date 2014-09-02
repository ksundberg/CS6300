#include "Read.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::Read::emit()
{
  auto block = std::make_shared<BasicBlock>();
  return std::make_pair(block, block);
}
