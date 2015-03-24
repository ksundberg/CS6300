#include "SuperBlockClone.hpp"
#include "FlowGraph.h"

void cs6300::superBlockClone(cs6300::FlowGraph original)
{
  initParents(original);

  for (auto bb : allBlocks(original))
  {
    if (bb->branchTo == nullptr)
    {
      bool cloneable = true;
      for(auto&&p : bb->parents)
      {
        if(p->branchTo != nullptr)
        {
            cloneable = false;
            break;
        }
      }

      if(!cloneable)
          continue;

      // clone block into parents
      for(auto&&p : bb->parents)
      {
        p->jumpTo = bb->jumpTo;
        for (auto& inst : bb->instructions)
          p->instructions.emplace_back(inst);
      }
    }
  }
}
