#include "CallExpression.hpp"
#include "AST/SymbolTable.hpp"
#include "logger.h"

cs6300::CallExpression::CallExpression(
    int l, std::vector<std::shared_ptr<Expression>> args, std::shared_ptr<Type> t)
    : Expression()
    , funcLabel(l)
    , actualArguments(args)
    , returnType(t)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::CallExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();

  result->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::StoreFrame, 0, 0, 0));

  int offset = 0;
  for(auto&& arg:actualArguments)
  {
    auto code = arg->emit();
    std::copy(code->instructions.begin(),
              code->instructions.end(),
              std::back_inserter(result->instructions));
    offset += arg->type()->size();
    result->instructions.emplace_back(ThreeAddressInstruction::CopyArgument,
                                      cs6300::STACK,
                                      arg->getLabel(),
                                      offset);
  }

  result->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::CallFunction, 0, funcLabel, 0)); // set fp and jump
  result->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::Return, getLabel(), 0, 0)); // get return value into register

  // TODO: move sp back according to arguments size

  result->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::RestoreFrame, 0, 0, 0));

  return result;
}

std::shared_ptr<cs6300::Type> cs6300::CallExpression::type() const
{
  return returnType;
}

int cs6300::CallExpression::value() const { return 0; }

bool cs6300::CallExpression::isConst() const { return false; }

