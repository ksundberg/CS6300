#include "CallExpression.hpp"

cs6300::CallExpression::CallExpression(
    std::string n, std::vector<std::shared_ptr<Expression>> args)
    : Expression()
    , name(n)
    , actualArguments(args)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::CallExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  for(auto&& arg:actualArguments)
  {
    auto code = arg->emit();
    std::copy(code->instructions.begin(),
              code->instructions.end(),
              std::back_inserter(result->instructions));
    result->instructions.emplace_back(ThreeAddressInstruction::CopyArgument,
                                      0 /*TODO:Placeholder for argument*/,
                                      arg->getLabel(),
                                      0);
  }
  result->instructions.emplace_back(ThreeAddressInstruction::CallFunction,
                                    getLabel(),
                                    0 /*TODO:Placeholder for function*/,
                                    0);
  return result;
}

std::shared_ptr<cs6300::Type> cs6300::CallExpression::type() const
{
  return nullptr;
}

int cs6300::CallExpression::value() const { return 0; }

bool cs6300::CallExpression::isConst() const { return false; }

