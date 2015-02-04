#include "Call.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

cs6300::FlowGraph cs6300::Call::emit()
{
  auto result = std::make_shared<BasicBlock>();
  int stack_offset = symbolTable->stackSpace();

  result->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::StoreFrame, 0, stack_offset, 0));

  int offset = 0;
  for (auto&& arg : arguments)
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
                            label,
                            offset)); // set fp and jump

  result->instructions.push_back(ThreeAddressInstruction(
    ThreeAddressInstruction::RestoreFrame, 0, stack_offset, 0));

  return std::make_pair(result, result);
}

std::string cs6300::Call::ClassName() const
{
  return "Call";
}

std::string cs6300::Call::name() const
{
  return "\"Call " + _name + "\"";
}

std::vector<std::string> cs6300::Call::_ASTLines() const
{
  std::vector<std::string> lines;
  return join(arguments, lines, id());
}
