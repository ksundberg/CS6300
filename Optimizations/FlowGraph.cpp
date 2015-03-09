#include "FlowGraph.h"
#include <unordered_set>
#include <stack>

void cs6300::initParents(cs6300::FlowGraph graph)
{
  auto&& blocks = allBlocks(graph);
  for (auto& bb : blocks)
    bb->parents.clear();

  for (auto& bb : blocks)
  {
    if (bb->jumpTo) bb->jumpTo->parents.insert(bb);
    if (bb->branchTo) bb->branchTo->parents.insert(bb);
  }
}

std::list<std::shared_ptr<cs6300::BasicBlock>> cs6300::allBlocks(
  cs6300::FlowGraph graph)
{
  std::unordered_set<std::shared_ptr<cs6300::BasicBlock>> visited;
  std::list<std::shared_ptr<cs6300::BasicBlock>> blocks;
  std::stack<std::shared_ptr<cs6300::BasicBlock>> q;
  q.push(graph.first);

  while (q.size())
  {
    auto&& bb = std::move(q.top());

    if (visited.count(bb))
    {
      q.pop();
      blocks.emplace_back(bb);
      continue;
    }

    visited.insert(bb);

    if (bb->branchTo && !visited.count(bb->branchTo)) q.push(bb->branchTo);
    if (bb->jumpTo && !visited.count(bb->jumpTo)) q.push(bb->jumpTo);
  }
  return blocks;
}

std::string cs6300::flowGraphDot(cs6300::FlowGraph graph)
{
  std::vector<std::string> edges;
  for (auto&& v : allBlocks(graph))
  {
    if (v->branchTo)
      edges.emplace_back("\t" + v->getLabel() + " -> " +
                         v->branchTo->getLabel() + " [label=br];");
    if (v->jumpTo)
      edges.emplace_back("\t" + v->getLabel() + " -> " + v->jumpTo->getLabel() +
                         " [label=j];");
  }

  std::string digraph = "digraph G {";

  for (auto& s : edges)
    digraph += s + "\n";

  return digraph + "\n}";
}
