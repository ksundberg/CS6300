#ifndef CS6300_OPTIMIZER_HPP
#define CS6300_OPTIMIZER_HPP

#include <memory>

namespace cs6300
{
class Program;
class BasicBlock;
/*Returns the Abstract Syntax Tree that is the result of parsing filename*/
std::shared_ptr<Program> optimizer(std::shared_ptr<Program>);
/*Returns an improved control flow graph*/
std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>> optimizer(
  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>>);
}

#endif
