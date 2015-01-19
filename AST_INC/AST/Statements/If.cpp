#include "If.hpp"
#include <memory>

cs6300::FlowGraph cs6300::IfStatement::emit()
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

std::string cs6300::IfStatement::ClassName() const
{
  return "If";
}

std::vector<std::string> cs6300::IfStatement::_ASTLines() const
{
  std::vector<std::string> lines;
  int count = 0;
  for (auto& clause : clauses)
  {
    std::string clausesId = "clauses" + std::to_string(count) + id();

    // first clause is if statement other clauses are else-if statements
    if (count == 0)
      lines.emplace_back(clausesId + " [label=If]");
    else
      lines.emplace_back(clausesId + " [label=ElseIf]");

    lines.emplace_back(id() + " -> " + clausesId);

    join(clause.first, lines, clausesId);
    join(clause.second, lines, clausesId);

    count++;
  }
  if (elseClause.size())
  {
    std::string elseId = "else" + id();
    lines.emplace_back(elseId + " [label=Else]");
    lines.emplace_back(id() + " -> " + elseId);
    join(elseClause, lines, elseId);
  }
  return lines;
}
