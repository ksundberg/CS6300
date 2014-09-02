#ifndef CS6300_STATEMENTS_HPP
#define CS6300_STATEMENTS_HPP

#include "AST/BasicBlock.hpp"

namespace cs6300
{
  class Statement
  {
  public:
    virtual ~Statement()=default;
    virtual std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>>
    emit() = 0;
  };

  std::pair<std::shared_ptr<BasicBlock>, std::shared_ptr<BasicBlock>>
  emitList(std::vector<std::shared_ptr<Statement>> statements);
}

#endif
