#ifndef CS6300_BASIC_BLOCK_HPP
#define CS6300_BASIC_BLOCK_HPP

#include <memory>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "ThreeAddressInstruction.hpp"
#include "RegisterScope.hpp"

namespace cs6300
{
class BasicBlock
{

public:
  std::vector<ThreeAddressInstruction> instructions;
  std::shared_ptr<BasicBlock> jumpTo;
  std::shared_ptr<BasicBlock> branchTo;
  std::vector<std::shared_ptr<BasicBlock>> parents;
  int branchOn;
  std::string getLabel();

  // Used for register allocation
  cs6300::RegisterScope m;
  void initSets();
  static cs6300::RegisterScope scope(ThreeAddressInstruction tal);
  void remap(std::map<int, int> m);

private:
  std::string label;
};
}
#endif
