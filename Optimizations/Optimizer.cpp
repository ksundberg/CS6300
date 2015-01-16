#include "Optimizer.hpp"
#include "MaximizeBlocks/MaximizeBlocks.hpp"
#include "logger.h"

#include <sstream>

/*Add new AST based optimizations here*/
std::shared_ptr<cs6300::Program> cs6300::optimizer(
  std::shared_ptr<cs6300::Program> original)
{
  return original;
}
/*Add new control flow graph based optimizations here*/
cs6300::FlowGraph cs6300::optimizer(cs6300::FlowGraph original)
{
  LOG(DEBUG) << "Optimizing flowgraph";
  maximizeBlocks(original);
  return original;
}
