#ifndef CS6300_BACK_END_HPP
#define CS6300_BACK_END_HPP

#include <memory>
#include <vector>
#include <set>

namespace cs6300
{
  class IntermediateRepresentationProgram;
  class BasicBlock;

  void writeMIPS(std::shared_ptr<IntermediateRepresentationProgram>,std::string filename);
  void locRegAlloc(std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>>);
  bool pushUp(std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>);
  std::vector<std::set<int>> regDeps(std::shared_ptr<BasicBlock>);
  std::set<int> allReg(std::shared_ptr<BasicBlock>);
}
#endif
