#include <algorithm>

#include "MaximizeBlocks.hpp"
#include "../AST_INC/AST/BasicBlock.hpp"
#include "../AST_INC/AST/ThreeAddressInstruction.hpp"

std::shared_ptr<cs6300::VisitedBlocks> cs6300::VisitedBlocks::m_instance;

bool cs6300::VisitedBlocks::isVisited(std::shared_ptr<BasicBlock> block){
  // if in vector return true
  // else put in vector and return false
  std::vector<std::string>::iterator it;

  // get block as address string
  std::ostringstream stream;
  stream << block;
  std::string address = stream.str();

  it = find(m_blocks.begin(), m_blocks.end(), address);
  if(it == m_blocks.end()){
    m_blocks.push_back(address);
    return false;
  }
  return true;
}
void cs6300::VisitedBlocks::reset(){
  // reset vector
  m_blocks.clear();
}

std::shared_ptr<cs6300::NumParents> cs6300::NumParents::m_instance;

void cs6300::NumParents::addParent(std::shared_ptr<BasicBlock> block){
  std::string sig = getString(block);

  int numParents = 0;
  if(parents[sig]){
    numParents += parents[sig];
  }
  numParents++;
  parents[sig] = numParents;
}

int cs6300::NumParents::getNumParents(std::shared_ptr<BasicBlock> block){
  std::string sig = getString(block);

  if(!parents[sig]) return 0;

  return parents[sig];
}

void cs6300::NumParents::reset(){
  parents.clear();
}

std::string cs6300::NumParents::getString(std::shared_ptr<BasicBlock> block){
  std::ostringstream stream;
  stream << block;
  return stream.str();
}

void cs6300::maximizeBlocks(std::pair<std::shared_ptr<cs6300::BasicBlock>,
                            std::shared_ptr<cs6300::BasicBlock>> original)
{
  // do something
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
  if(block->branchTo != nullptr && block->jumpTo == nullptr && np->getNumParents(block->branchTo) == 1){
    // merge branchTo to this block
    for(auto inst : block->branchTo->instructions){
      block->instructions.push_back(inst);
    }
    block->jumpTo = block->branchTo->jumpTo;
    block->branchTo = block->branchTo->branchTo;
  }else if(block->jumpTo != nullptr && block->branchTo == nullptr && np->getNumParents(block->jumpTo) == 1){
    // merge jumpTo to this block
    for(auto inst : block->jumpTo->instructions){
      block->instructions.push_back(inst);
    }
    block->branchTo = block->jumpTo->branchTo;
    block->jumpTo = block->jumpTo->jumpTo;
  }

}