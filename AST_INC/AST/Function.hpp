#ifndef CS6300_FUNCTION_HPP
#define CS6300_FUNCTION_HPP

#include <memory>
#include "BasicBlock.hpp"

namespace cs6300
{
  class Function
  {
    std::shared_ptr<BasicBlock> code;
  };
}
#endif
