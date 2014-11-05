#include "Repeat.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::RepeatStatement::emit()
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
