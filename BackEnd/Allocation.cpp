#include "Allocation.hpp"
#include "AST/Program.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "Optimizations/FlowGraph.h"
#include <algorithm>

struct RegColorNode
{
  int color;
  std::set<RegColorNode*> nodes;
  std::set<int> cant;
};

void cs6300::locRegAlloc(cs6300::FlowGraph graph)
{
  int count = 0;

  for (auto&& v : allBlocks(graph))
    v->initSets();

  // propogate block meta
  bool change;
  do
  {
    change = false;
    for (auto&& v : allBlocks(graph))
    {
      if (pushUp(v, v->jumpTo)) change = true;

      if (pushUp(v, v->branchTo)) change = true;
    }
  } while (change);

  std::map<int, RegColorNode*> nodes;
  for (auto&& cur : allBlocks(graph))
  {
    auto t = regDeps(cur);
    auto s = std::set<std::set<int>>(t.begin(), t.end());
    for (auto& v : s)
    {
      for (int reg : v)
      {
        for (int nreg : v)
        {
          if (!nodes.count(nreg)) nodes[nreg] = new RegColorNode{-1};
          nodes[reg]->nodes.insert(nodes[nreg]);
        }
      }
    }
  }

  std::map<int, int> regRemap;
  // color nodes
  for (auto& p : nodes)
  {
    if (p.second->color != -1)
    {
      continue; // already colored
    }

    for (int i = START_REGISTER;; i++)
    {
      if (!p.second->cant.count(i))
      {
        p.second->color = i;
        regRemap[p.first] = i;
        // remapping p.first to i
        for (auto& n : p.second->nodes)
        {
          n->cant.insert(i);
        }
        break;
      }
      if (i == END_REGISTER)
      {
        std::cerr << "Too many colors tried" << std::endl;
        break;
      }
    }
  }

  for (auto&& v : allBlocks(graph))
  {
    v->remap(regRemap);
  }
}

std::vector<std::set<int>> cs6300::regDeps(
  std::shared_ptr<cs6300::BasicBlock> b)
{
  std::vector<std::set<int>> res;

  std::set<int> prop = b->m.alive;
  for (auto i = b->instructions.rbegin(); i != b->instructions.rend(); i++)
  {
    auto m = BasicBlock::scope(*i);
    prop.insert(m.used.begin(), m.used.end());
    std::vector<int> t;
    std::set_difference(
      prop.begin(), prop.end(), m.dead.begin(), m.dead.end(), back_inserter(t));
    prop = std::set<int>(t.begin(), t.end());
    res.insert(res.begin(), prop);
  }

  if (b->branchTo)
  {
    res.push_back(std::set<int>{b->branchOn});
  };

  return res;
}

bool cs6300::pushUp(std::shared_ptr<cs6300::BasicBlock> parent,
                    std::shared_ptr<cs6300::BasicBlock> child)
{
  if (!child) return false;

  std::vector<int> t;
  set_difference(child->m.alive.begin(),
                 child->m.alive.end(),
                 parent->m.dead.begin(),
                 parent->m.dead.end(),
                 back_inserter(t));

  int origSize = parent->m.alive.size();
  parent->m.alive.insert(t.begin(), t.end());

  return origSize != parent->m.alive.size();
}
