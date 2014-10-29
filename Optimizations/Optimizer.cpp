#include "Optimizer.hpp"
#include "MaximizeBlocks/MaximizeBlocks.hpp"

#include <sstream>

/*Add new AST based optimizations here*/
std::shared_ptr<cs6300::Program>
cs6300::optimizer(std::shared_ptr<cs6300::Program> original)
{
  return original;
}
/*Add new control flow graph based optimizations here*/
std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::optimizer(std::pair<std::shared_ptr<cs6300::BasicBlock>,
                            std::shared_ptr<cs6300::BasicBlock>> original)
{
  maximizeBlocks(original);
  return original;
}

/*std::string cs6300::getBlockSig(std::shared_ptr<BasicBlock> block){
  std::ostringstream stream;
  stream << block;
  return stream.str();
}*/
