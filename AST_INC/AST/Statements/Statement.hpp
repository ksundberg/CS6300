#ifndef CS6300_STATEMENTS_HPP
#define CS6300_STATEMENTS_HPP

#include "AST/BasicBlock.hpp"

namespace cs6300
{
  class Statement
  {
  public:
    virtual ~Statement()=default;
    virtual cs6300::FlowGraph emit() = 0;
  };

  cs6300::FlowGraph emitList(std::vector<std::shared_ptr<Statement>> statements);
}

#endif
