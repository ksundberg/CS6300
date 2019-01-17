#ifndef CS6300_MAXIMIZEBLOCKS_HPP
#define CS6300_MAXIMIZEBLOCKS_HPP

#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "AST/BasicBlock.hpp"
#include "AST/ThreeAddressInstruction.hpp"

namespace cs6300
{
void maximizeBlocks(FlowGraph);

void buildParentCounts(std::shared_ptr<BasicBlock> block);
void combineBlocks(std::shared_ptr<BasicBlock>);
}

#endif
