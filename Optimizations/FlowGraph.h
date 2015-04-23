#ifndef __FLOWGRAPH_H_
#define __FLOWGRAPH_H_

#include "AST/BasicBlock.hpp"
#include <list>

namespace cs6300
{

/* Generate a Dot file for the given cs6300::FlowGraph */
std::string flowGraphDot(cs6300::FlowGraph);

/* Initialize parent lists for basic blocks */
void initParents(cs6300::FlowGraph graph);

/* return a list of all basic blocks in postorder (bottom-up traversing).
 * use reverse iterator for reverse-postorder top down traversing of the
 * flowgraph */
std::list<std::shared_ptr<cs6300::BasicBlock>> allBlocks(
  cs6300::FlowGraph graph);
}

#endif // __FLOWGRAPH_H_
