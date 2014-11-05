#include "For.hpp"
#include "Assignment.hpp"
#include "AST/LValue.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/Type.hpp"
#include "AST/Expressions/LoadExpression.hpp"
#include "AST/Expressions/PredecessorExpression.hpp"
#include "AST/Expressions/SuccessorExpression.hpp"
#include "AST/Expressions/LtExpression.hpp"

std::pair<std::shared_ptr<cs6300::BasicBlock>,
          std::shared_ptr<cs6300::BasicBlock>>
cs6300::ForStatement::emit()
{
  Assignment assign(std::make_shared<IdAccess>(loopVariable,table), begin);
  auto init = assign.emit();
  auto compareExpr = std::make_shared<LtExpression>(
      std::make_shared<LoadExpression>(
          std::make_shared<IdAccess>(loopVariable,table)),
      end);
  std::shared_ptr<Expression> incr = std::make_shared<PredecessorExpression>(
      std::make_shared<LoadExpression>(
          std::make_shared<IdAccess>(loopVariable,table)));

  if(direction == TO)
  {
      compareExpr = std::make_shared<LtExpression>(
              end,
              std::make_shared<LoadExpression>(
                  std::make_shared<IdAccess>(loopVariable,table)));

      incr = std::make_shared<SuccessorExpression>(
              std::make_shared<LoadExpression>(
                  std::make_shared<IdAccess>(loopVariable,table)));
  }

  Assignment store(std::make_shared<IdAccess>(loopVariable,table), incr);
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
