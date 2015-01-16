#ifndef CS6300_BACK_END_HPP
#define CS6300_BACK_END_HPP

#include <memory>
#include <vector>
#include <set>

#include "AST/BasicBlock.hpp"

namespace cs6300
{
class IntermediateRepresentationProgram;

void writeMIPS(std::shared_ptr<IntermediateRepresentationProgram>,
               std::string filename);
void locRegAlloc(cs6300::FlowGraph);
bool pushUp(std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>);
std::vector<std::set<int>> regDeps(std::shared_ptr<BasicBlock>);
}
#endif
