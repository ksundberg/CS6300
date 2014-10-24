#ifndef CS6300_MAXIMIZEBLOCKS_HPP
#define CS6300_MAXIMIZEBLOCKS_HPP

#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "../../AST_INC/AST/BasicBlock.hpp"
#include "../../AST_INC/AST/ThreeAddressInstruction.hpp"

namespace cs6300
{
  void maximizeBlocks(std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>>);

  void traverse(std::shared_ptr<BasicBlock> block);
}


#endif

