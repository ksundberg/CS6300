#include "Expression.hpp"

int cs6300::Expression::getLabel() const
{
  if (label == -1)
  {
    label = getNextLabel();
  }
  return label;
}

std::string cs6300::Expression::id() const
{
  return "Expression" + std::to_string(getLabel());
}

std::string cs6300::Expression::node() const
{
  return id() + " [label=" + name() + "]";
}

int cs6300::Expression::getNextLabel(bool reset)
{
  static int curLabel = 0;
  if(reset)
  {
      curLabel = 0;
      return 0;
  }
  return ++curLabel;
}

std::shared_ptr<cs6300::BasicBlock> cs6300::emitBinaryOp(
  cs6300::ThreeAddressInstruction::Type op,
  int result,
  std::shared_ptr<cs6300::Expression> lhs,
  std::shared_ptr<cs6300::Expression> rhs)
{
  auto resultCode = std::make_shared<cs6300::BasicBlock>();
  if (!lhs) return nullptr;
  if (!rhs) return nullptr;
  auto lhsCode = lhs->emit();
  auto rhsCode = rhs->emit();
  std::copy(lhsCode->instructions.begin(),
            lhsCode->instructions.end(),
            std::back_inserter(resultCode->instructions));
  std::copy(rhsCode->instructions.begin(),
            rhsCode->instructions.end(),
            std::back_inserter(resultCode->instructions));
  resultCode->instructions.emplace_back(
    op, result, lhs->getLabel(), rhs->getLabel());
  return resultCode;
}

std::vector<std::string>& cs6300::join(
  const std::shared_ptr<cs6300::Expression>& expr,
  std::vector<std::string>& dst,
  std::string id,
  std::string lbl)
{
  dst.emplace_back(expr->node());
  if (lbl == "")
    dst.emplace_back(id + " -> " + expr->id());
  else
    dst.emplace_back(id + " -> " + expr->id() + "[label=" + lbl + "]");
  return copy(expr->ASTDot(), dst);
}

std::vector<std::string>& cs6300::join(
  const std::vector<std::shared_ptr<cs6300::Expression>>& exprs,
  std::vector<std::string>& dst,
  std::string id)
{
  for (auto& expr : exprs)
    join(expr, dst, id);
  return dst;
}
