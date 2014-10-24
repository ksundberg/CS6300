#include "NumParents.hpp"

#include <sstream>

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
