#ifndef CS6300_ALLOCATION_HPP
#define CS6300_ALLOCATION_HPP

#include <memory>
#include <vector>
#include <set>

#include "AST/BasicBlock.hpp"

namespace cs6300
{
class BasicBlock;

void locRegAlloc(cs6300::FlowGraph);
bool pushUp(std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>);
std::vector<std::set<int>> regDeps(std::shared_ptr<BasicBlock>);
}
#endif
