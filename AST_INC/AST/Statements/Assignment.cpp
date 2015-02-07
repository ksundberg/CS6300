#include "Assignment.hpp"
#include "AST/Type.hpp"
#include "AST/ThreeAddressInstruction.hpp"

cs6300::FlowGraph cs6300::Assignment::emit()
{
  auto block = std::make_shared<BasicBlock>();
  block->instructions.emplace_back(
    ThreeAddressInstruction("Begin Assignment", file, line));
  auto addr = lval->address();
  auto exprblock = expr->emit();
  auto addrblock = addr->emit();
  block->instructions.emplace_back(
    ThreeAddressInstruction("Begin exprblock", file, line));
  std::copy(exprblock->instructions.begin(),
            exprblock->instructions.end(),
            std::back_inserter(block->instructions));
  block->instructions.emplace_back(
    ThreeAddressInstruction("Begin addrblock", file, line));
  std::copy(addrblock->instructions.begin(),
            addrblock->instructions.end(),
            std::back_inserter(block->instructions));
  block->instructions.emplace_back(
    ThreeAddressInstruction("end addrblock", file, line));

  if (!(std::dynamic_pointer_cast<ArrayType>(lval->type()) ||
        std::dynamic_pointer_cast<RecordType>(lval->type())))
  {
    block->instructions.emplace_back(ThreeAddressInstruction::StoreMemory,
                                     expr->getLabel(),
                                     addr->getLabel(),
                                     0);
  }
  else
  {
    auto tlabel = Expression::getNextLabel();
    block->instructions.emplace_back(ThreeAddressInstruction(
      "Deep copying. Size " + std::to_string(expr->type()->size())));

    int max = expr->type()->size() / 4;
    for (int i = 0; i < max; i++)
    {
      // load from rhs, store to lhs
      block->instructions.emplace_back(
        ThreeAddressInstruction::LoadMemory, tlabel, expr->getLabel(), i * 4);

      block->instructions.emplace_back(
        ThreeAddressInstruction::StoreMemory, tlabel, addr->getLabel(), i * 4);
    }
  }

  block->instructions.emplace_back(ThreeAddressInstruction("End Assignment"));
  return std::make_pair(block, block);
}

std::string cs6300::Assignment::ClassName() const
{
  return "Assign";
}

std::vector<std::string> cs6300::Assignment::_ASTLines() const
{
  std::vector<std::string> lines;
  join(lval, lines, id());
  return join(expr, lines, id());
}
