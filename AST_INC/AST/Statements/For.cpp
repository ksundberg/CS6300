#include "For.hpp"
#include "Assignment.hpp"
#include "AST/LValue.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/Type.hpp"
#include "AST/Expressions/LoadExpression.hpp"
#include "AST/Expressions/PredecessorExpression.hpp"
#include "AST/Expressions/SuccessorExpression.hpp"
#include "AST/Expressions/LtExpression.hpp"

cs6300::FlowGraph cs6300::ForStatement::emit()
{
  Assignment assign(std::make_shared<IdAccess>(loopVariable, table), begin);
  auto init = assign.emit();
  auto compareExpr = std::make_shared<LtExpression>(
    std::make_shared<LoadExpression>(
      std::make_shared<IdAccess>(loopVariable, table)),
    end);
  std::shared_ptr<Expression> incr =
    std::make_shared<PredecessorExpression>(std::make_shared<LoadExpression>(
      std::make_shared<IdAccess>(loopVariable, table)));

  if (direction == TO)
  {
    compareExpr = std::make_shared<LtExpression>(
      end,
      std::make_shared<LoadExpression>(
        std::make_shared<IdAccess>(loopVariable, table)));

    incr =
      std::make_shared<SuccessorExpression>(std::make_shared<LoadExpression>(
        std::make_shared<IdAccess>(loopVariable, table)));
  }

  Assignment store(std::make_shared<IdAccess>(loopVariable, table), incr);
  auto compare = compareExpr->emit();
  auto mutate = store.emit();
  auto b = emitList(body);
  auto endFor = std::make_shared<cs6300::BasicBlock>();
  init.second->jumpTo = compare;
  compare->jumpTo = b.first;
  b.second->jumpTo = mutate.first;
  mutate.second->jumpTo = compare;
  compare->branchTo = endFor;
  compare->branchOn = compareExpr->getLabel();

  return std::make_pair(init.first, endFor);
}

std::string cs6300::ForStatement::ClassName() const
{
  return "For";
}

std::vector<std::string> cs6300::ForStatement::_ASTLines() const
{
  std::vector<std::string> lines;
  auto var = std::make_shared<IdAccess>(loopVariable, table);
  join(var, lines, id());
  join(begin, lines, var->id(), "from");
  std::string dStr = direction == TO ? "to" : "downto";
  join(end, lines, var->id(), dStr);
  return join(body, lines, id());
}
