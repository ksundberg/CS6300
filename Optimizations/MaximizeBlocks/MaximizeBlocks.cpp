#include <algorithm>

#include "MaximizeBlocks.hpp"
#include "../../AST_INC/AST/BasicBlock.hpp"
#include "../../AST_INC/AST/ThreeAddressInstruction.hpp"
#include "VisitedBlocks.hpp"
#include "NumParents.hpp"

void cs6300::maximizeBlocks(std::pair<std::shared_ptr<cs6300::BasicBlock>,
                            std::shared_ptr<cs6300::BasicBlock>> original)
{
  // traverse the blocks
  cs6300::traverse(original.first);
  cs6300::traverse(original.second);
}

void cs6300::traverse(std::shared_ptr<BasicBlock> block){
  auto vb = VisitedBlocks::instance();
  auto np = NumParents::instance();

  if(vb->isVisited(block)){
    return;
  }

  if(block->branchTo != nullptr){
    // add parents on the way down...
    np->addParent(block->branchTo);
    traverse(block->branchTo);
  }
  if(block->jumpTo != nullptr){
    // add parents on the way down...
    np->addParent(block->jumpTo);
    traverse(block->jumpTo);
  }
  // determine if they can be merged on the way back up...
  if(block->jumpTo != nullptr && block->branchTo == nullptr && np->getNumParents(block->jumpTo) == 1){
    // merge jumpTo to this block
    for(auto inst : block->jumpTo->instructions){
      block->instructions.push_back(inst);
    }
    block->branchTo = block->jumpTo->branchTo;
    block->jumpTo = block->jumpTo->jumpTo;
  }

}
