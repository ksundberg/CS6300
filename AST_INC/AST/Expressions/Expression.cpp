#include "Expression.hpp"

int cs6300::Expression::getLabel() const
{
  if (label == -1)
  {
    label = getNextLabel();
  }
  return label;
}

int cs6300::Expression::getNextLabel()
{
  static int curLabel = 0;
  return ++curLabel;
}

std::shared_ptr<cs6300::BasicBlock>
cs6300::emitBinaryOp(cs6300::ThreeAddressInstruction::Type op,
                     int result,
                     std::shared_ptr<cs6300::Expression> lhs,
                     std::shared_ptr<cs6300::Expression> rhs)
{
  auto resultCode = std::make_shared<cs6300::BasicBlock>();
  if(!lhs) return nullptr;
  if(!rhs) return nullptr;
  auto lhsCode = lhs->emit();
  auto rhsCode = rhs->emit();
  std::copy(lhsCode->instructions.begin(),
            lhsCode->instructions.end(),
            std::back_inserter(resultCode->instructions));
  std::copy(rhsCode->instructions.begin(),
            rhsCode->instructions.end(),
            std::back_inserter(resultCode->instructions));
  resultCode->instructions.emplace_back(cs6300::ThreeAddressInstruction::Add,
                                        result,
                                        lhs->getLabel(),
                                        rhs->getLabel());
  return resultCode;

}
