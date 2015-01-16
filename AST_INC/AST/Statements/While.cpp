#include "While.hpp"

cs6300::FlowGraph cs6300::WhileStatement::emit()
{
  auto b = emitList(body);
  auto expr = condition->emit();
  b.second->jumpTo = expr;
  auto exitBlock = std::make_shared<BasicBlock>();
  expr->jumpTo = exitBlock;
  expr->branchTo = b.first;
  expr->branchOn = condition->getLabel();
  return std::make_pair(expr, exitBlock);
}
