#include "Statement.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::emitList(std::vector<std::shared_ptr<Statement>> statements)
{
  auto entry = std::make_shared<BasicBlock>();
  auto current = entry;
  for (auto&& s : statements)
  {
    if (s)
    {
      auto subgraph = s->emit();
      current->jumpTo = subgraph.first;
      current = subgraph.second;
    }
  }
  return std::make_pair(entry, current);
}
