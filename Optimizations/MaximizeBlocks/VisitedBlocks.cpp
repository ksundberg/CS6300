#include <algorithm>
#include <vector>
#include <sstream>

#include "VisitedBlocks.hpp"
#include "../../AST_INC/AST/BasicBlock.hpp"

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
