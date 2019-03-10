#ifndef __cpsl__TraverseBasicBlock__
#define __cpsl__TraverseBasicBlock__

#include "AST/BasicBlock.hpp"
#include <functional>

namespace cs6300
{
std::map<std::shared_ptr<cs6300::BasicBlock>,
         std::vector<std::shared_ptr<cs6300::BasicBlock>>>
buildParentMap(std::shared_ptr<cs6300::BasicBlock> flowGraph);

void traverseFlowGraph(
  std::shared_ptr<cs6300::BasicBlock> sourceBlock,
  std::function<bool(std::shared_ptr<cs6300::BasicBlock>)> action);

/* Generate a Dot file for the given cs6300::FlowGraph */
std::string flowGraphDot(cs6300::FlowGraph);

std::set<std::shared_ptr<cs6300::BasicBlock>> allBlocks(
  cs6300::FlowGraph graph);
}

#endif /* defined(__cpsl__TraverseBasicBlock__) */
