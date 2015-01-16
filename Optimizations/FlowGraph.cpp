#include "FlowGraph.h"

// I should probably just add this as a property of basic block but I am
// paranoid about code merges.
std::map<std::shared_ptr<cs6300::BasicBlock>,
         std::vector<std::shared_ptr<cs6300::BasicBlock>>>
cs6300::buildParentMap(std::shared_ptr<cs6300::BasicBlock> flowGraph)
{
  std::map<std::shared_ptr<cs6300::BasicBlock>,
           std::vector<std::shared_ptr<cs6300::BasicBlock>>> parentMap;
  traverseFlowGraph(
    flowGraph,
    [&parentMap](std::shared_ptr<cs6300::BasicBlock> block) -> bool
    {
      if (block->jumpTo != nullptr)
      {
        parentMap[block->jumpTo].push_back(block);
      }

      if (block->branchTo != nullptr)
      {
        parentMap[block->branchTo].push_back(block);
      }
      return true;

    });

  return parentMap;
}

void cs6300::traverseFlowGraph(
  std::shared_ptr<cs6300::BasicBlock> sourceBlock,
  std::function<bool(std::shared_ptr<cs6300::BasicBlock>)> action)
{
  auto searchBlock = sourceBlock;
  std::set<std::shared_ptr<cs6300::BasicBlock>> seenList;
  std::vector<std::shared_ptr<cs6300::BasicBlock>> visitStack;

  visitStack.push_back(nullptr);

  while (searchBlock != nullptr)
  {

    if (seenList.find(searchBlock) == seenList.end())
    {
      if (!action(searchBlock)) continue;
      seenList.insert(searchBlock);
      visitStack.push_back(searchBlock);
    }

    if (searchBlock->jumpTo != nullptr &&
        seenList.find(searchBlock->jumpTo) == seenList.end())
    {
      searchBlock = searchBlock->jumpTo;
    }
    else if (searchBlock->branchTo != nullptr &&
             seenList.find(searchBlock->branchTo) == seenList.end())
    {
      searchBlock = searchBlock->branchTo;
    }
    else
    {
      searchBlock = visitStack.back();
      visitStack.pop_back();
    }
  }
}

std::set<std::shared_ptr<cs6300::BasicBlock>> cs6300::allBlocks(
  cs6300::FlowGraph graph)
{
  std::set<std::shared_ptr<cs6300::BasicBlock>> all;
  std::vector<std::shared_ptr<cs6300::BasicBlock>> todo;
  auto at = graph.first;
  do
  {
    if (todo.size())
    {
      at = todo.back();
      todo.pop_back();
    }
    while (at)
    {
      if (!all.count(at))
      {
        all.insert(at);
      }
      else
      {
        break;
      }
      if (at->branchTo && !all.count(at->branchTo))
      {
        at->branchTo->parents.insert(at);
        todo.emplace_back(at->branchTo);
      }
      if (at->jumpTo)
      {
        at->jumpTo->parents.insert(at);
      }
      at = at->jumpTo;
    }
  } while (todo.size());
  return all;
}

std::string cs6300::flowGraphDot(cs6300::FlowGraph graph)
{
  std::vector<std::string> edges;
  for (auto&& v : allBlocks(graph))
  {
    if (v->branchTo)
      edges.emplace_back("\t" + v->getLabel() + " -> " +
                         v->branchTo->getLabel() + " [label=\"br\"];");
    if (v->jumpTo)
      edges.emplace_back("\t" + v->getLabel() + " -> " + v->jumpTo->getLabel() +
                         " [label=\"j\"];");
  }

  std::string digraph = "digraph G {";

  for (auto& s : edges)
    digraph += s + "\n";

  return digraph + "\n}";
}
