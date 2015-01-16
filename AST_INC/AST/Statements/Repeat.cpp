#include "Repeat.hpp"

cs6300::FlowGraph cs6300::RepeatStatement::emit()
{
  auto b = emitList(body);
  auto expr = condition->emit();
  b.second->jumpTo = expr;
  auto exitBlock = std::make_shared<BasicBlock>();
  expr->jumpTo = b.first;
  expr->branchTo = exitBlock;
  expr->branchOn = condition->getLabel();
  return std::make_pair(b.first, exitBlock);
}
