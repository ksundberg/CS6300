#include <algorithm>

#include "MaximizeBlocks.hpp"
#include "AST/BasicBlock.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "VisitedBlocks.hpp"
#include "NumParents.hpp"

void cs6300::maximizeBlocks(cs6300::FlowGraph& original)
{
  // traverse the blocks
  auto vb = VisitedBlocks::instance();
  vb->reset();
  buildParentCounts(original.first);
  vb->reset();
  combineBlocks(original.first, original);
}

void cs6300::buildParentCounts(std::shared_ptr<BasicBlock> block)
{
  auto vb = VisitedBlocks::instance();
  auto np = NumParents::instance();

  if (vb->isVisited(block)) return;

  if (block->branchTo != nullptr)
  {
    np->addParent(block->branchTo);
    buildParentCounts(block->branchTo);
  }
  if (block->jumpTo != nullptr)
  {
    np->addParent(block->jumpTo);
    buildParentCounts(block->jumpTo);
  }
}

void cs6300::combineBlocks(std::shared_ptr<BasicBlock> block, FlowGraph& graph)
{
  auto vb = VisitedBlocks::instance();
  auto np = NumParents::instance();

  if (vb->isVisited(block)) return;

  if (block->branchTo) combineBlocks(block->branchTo, graph);
  if (block->jumpTo) combineBlocks(block->jumpTo, graph);

  // determine if they can be merged on the way back up...
  if (block->jumpTo != nullptr && block->branchTo == nullptr &&
      np->getNumParents(block->jumpTo) == 1)
  {
    //Redirect the end of the graph
    if (block->jumpTo == graph.second)
    {
      graph.second = block;
    }

    // merge jumpTo to this block
    for (auto inst : block->jumpTo->instructions)
    {
      block->instructions.push_back(inst);
    }
    block->branchTo = block->jumpTo->branchTo;
    block->branchOn = block->jumpTo->branchOn;
    block->jumpTo = block->jumpTo->jumpTo;
  }
}
