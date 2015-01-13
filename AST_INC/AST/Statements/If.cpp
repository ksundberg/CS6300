#include "If.hpp"
#include <memory>

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::IfStatement::emit()
{
  auto entryPoint = std::make_shared<BasicBlock>();
  auto cur = entryPoint;
  auto exitPoint = std::make_shared<BasicBlock>();
  for (auto&& c : clauses)
  {
    auto exp = c.first->emit();
    auto thenPart = emitList(c.second);
    cur->jumpTo = exp;
    cur = exp;
    exp->branchOn = c.first->getLabel();
    exp->branchTo = thenPart.first;
    thenPart.second->jumpTo = exitPoint;
  }
  auto elsePart = emitList(elseClause);
  cur->jumpTo = elsePart.first;
  elsePart.second->jumpTo = exitPoint;
  return std::make_pair(entryPoint, exitPoint);
}
