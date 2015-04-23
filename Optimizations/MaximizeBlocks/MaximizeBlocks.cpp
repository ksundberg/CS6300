#include "MaximizeBlocks.hpp"
#include "FlowGraph.h"

void cs6300::maximizeBlocks(cs6300::FlowGraph original)
{
  initParents(original);

  for (auto bb : allBlocks(original))
  {
    if (bb->jumpTo != nullptr && bb->branchTo == nullptr &&
        bb->jumpTo->parents.size() == 1)
    {
      // merge jumpTo to this block
      for (auto&& inst : bb->jumpTo->instructions)
        bb->instructions.emplace_back(std::move(inst));

      auto erasing = bb->jumpTo;
      if (erasing->jumpTo)
      {
        erasing->jumpTo->parents.erase(erasing);
        erasing->jumpTo->parents.insert(bb);
      }

      bb->branchTo = erasing->branchTo;
      bb->branchOn = erasing->branchOn;
      bb->jumpTo = erasing->jumpTo;
    }
  }
}
