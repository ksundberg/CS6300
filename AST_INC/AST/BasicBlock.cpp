#include "BasicBlock.hpp"

std::string cs6300::BasicBlock::getLabel()
{

  static size_t blockNumber = 0;
  if (label.empty())
    label = std::string("BB") + std::to_string(++blockNumber);
  return label;
}
