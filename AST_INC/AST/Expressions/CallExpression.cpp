#include "CallExpression.hpp"
#include "logger.h"

cs6300::CallExpression::CallExpression(
  std::string n,
  int l,
  std::vector<std::shared_ptr<Expression>> args,
  std::shared_ptr<Type> t,
  std::shared_ptr<SymbolTable> symbols)
  : Expression()
  , _name(n)
  , funcLabel(l)
  , actualArguments(args)
  , returnType(t)
  , symbolTable(symbols)
{
}

std::shared_ptr<cs6300::BasicBlock> cs6300::CallExpression::emit() const
{
  auto result = std::make_shared<BasicBlock>();
  int stack_offset = symbolTable->stackSpace();

  result->instructions.push_back(
    ThreeAddressInstruction(ThreeAddressInstruction::StoreFrame, 0, stack_offset, 0));

  int offset = 0;
  for (auto&& arg : actualArguments)
  {
    auto code = arg->emit();
    std::copy(code->instructions.begin(),
              code->instructions.end(),
              std::back_inserter(result->instructions));

    offset -= arg->type()->size();
    if (std::dynamic_pointer_cast<ArrayType>(arg->type()))
    {
      auto tlabel = Expression::getNextLabel();
      result->instructions.emplace_back(ThreeAddressInstruction(
        "Deep copying. Size " + std::to_string(arg->type()->size())));

      int max = arg->type()->size() / 4;
      for (int i = 0; i < max; i++)
      {
        result->instructions.emplace_back(
          ThreeAddressInstruction::LoadMemory, tlabel, arg->getLabel(), i * 4);

        result->instructions.emplace_back(ThreeAddressInstruction::CopyArgument,
                                          cs6300::STACK,
                                          tlabel,
                                          offset + i * 4);
      }
    }
    else
    {
      result->instructions.emplace_back(ThreeAddressInstruction::CopyArgument,
                                        cs6300::STACK,
                                        arg->getLabel(),
                                        offset);
    }
  }

  result->instructions.push_back(
    ThreeAddressInstruction(ThreeAddressInstruction::CallFunction,
                            0,
                            funcLabel,
                            offset)); // set fp and jump

  result->instructions.push_back(
    ThreeAddressInstruction(ThreeAddressInstruction::RestoreFrame, 0, stack_offset, 0));

  result->instructions.push_back(
    ThreeAddressInstruction(ThreeAddressInstruction::Return,
                            getLabel(),
                            0,
                            0)); // get return value into register

  return result;
}

std::shared_ptr<cs6300::Type> cs6300::CallExpression::type() const
{
  return returnType;
}

int cs6300::CallExpression::value() const
{
  return 0;
}

bool cs6300::CallExpression::isConst() const
{
  return false;
}

std::string cs6300::CallExpression::name() const
{
  return "\"Call " + _name + "\"";
}

std::vector<std::string> cs6300::CallExpression::ASTDot() const
{
  std::vector<std::string> lines;
  return join(actualArguments, lines, id());
}
