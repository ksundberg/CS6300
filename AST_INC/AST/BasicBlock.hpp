#ifndef CS6300_BASIC_BLOCK_HPP
#define CS6300_BASIC_BLOCK_HPP

#include <memory>
#include <vector>
#include "ThreeAddressInstruction.hpp"

namespace cs6300
{
class BasicBlock
{
  std::vector<ThreeAddressInstruction> instructions;
};
}
#endif
