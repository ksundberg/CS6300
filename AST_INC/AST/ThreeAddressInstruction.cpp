#include "ThreeAddressInstruction.hpp"
#include "SymbolTable.hpp"
#include "logger.h"

bool cs6300::ThreeAddressInstruction::Verbose = false;

cs6300::ThreeAddressInstruction::ThreeAddressInstruction(Type t,
                                                         int d,
                                                         int s1,
                                                         int s2)
  : op(t), dest(d), src1(s1), src2(s2)
{
}

cs6300::ThreeAddressInstruction::ThreeAddressInstruction(std::string c)
  : op(Comment), dest(0), src1(0), src2(0), comment(c)
{
}

cs6300::ThreeAddressInstruction::ThreeAddressInstruction(std::string c,
                                                         std::string line,
                                                         std::string file)
  : op(Comment), dest(0), src1(0), src2(0), comment(c)
{
  comment += "(" + file + ":" + line + ")";
}

std::ostream& cs6300::operator<<(std::ostream& out,
                                 cs6300::ThreeAddressInstruction i)
{
  if (i.op == cs6300::ThreeAddressInstruction::Comment &&
      !ThreeAddressInstruction::Verbose)
    return out;

  out << "\t";
  switch (i.op)
  {
  case cs6300::ThreeAddressInstruction::Add:
    out << "add $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::AddValue:
    out << "addi $" << i.dest << ", $" << i.src1 << ", " << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::And:
    out << "and $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::CallFunction:
    out << "move $fp, $sp" << std::endl;
    out << "\taddi $sp, $sp, " << i.src2 << std::endl;
    out << "\tjal F" << i.src1 << std::endl;
    out << "\taddi $sp, $sp, " << -i.src2 << std::endl;
    break;
  case cs6300::ThreeAddressInstruction::Comment:
    out << "# " << i.comment;
    break;
  case cs6300::ThreeAddressInstruction::CopyArgument:
    out << "sw $" << i.src1 << ", " << i.src2 << "($" << i.dest
        << ") #copying argument";
    break;
  case cs6300::ThreeAddressInstruction::Divide:
    out << "div $" << i.src1 << ", $" << i.src2 << std::endl;
    out << "\tmflo $" << i.dest;
    break;
  case cs6300::ThreeAddressInstruction::IsEqual:
    out << "seq $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::IsGreater:
    out << "slt $" << i.dest << ", $" << i.src2 << ", $" << i.src1;
    break;
  case cs6300::ThreeAddressInstruction::IsGreaterEqual:
    out << "sge $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::IsLess:
    out << "slt $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::IsLessEqual:
    out << "sle $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::IsNotEqual:
    out << "sne $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::LoadLabel:
    if (i.dest != i.src1)
    {
      out << "addi $" << i.dest << ", $" << i.src1 << ", 0";
      out << " # Load a label";
    }
    break;
  case cs6300::ThreeAddressInstruction::LoadMemory:
    out << "lw $" << i.dest << ", " << i.src2 << "($" << i.src1 << ")";
    break;
  case cs6300::ThreeAddressInstruction::LoadMemoryOffset:
    if (i.src1 == cs6300::GLOBAL)
      out << "addi $" << i.dest << ", $gp, " << i.src2 << " # Load a global";
    else if (i.src1 == cs6300::STACK)
      out << "addi $" << i.dest << ", $sp, " << -i.src2 << " # Load a variable";
    else if (i.src1 == cs6300::FRAME)
      out << "addi $" << i.dest << ", $fp, " << -i.src2
          << " # Load a parameter";
    else
      LOG(FATAL) << "Programming error";

    break;

  case cs6300::ThreeAddressInstruction::LoadValue:
    out << "li $" << i.dest << ", " << i.src1;
    break;
  case cs6300::ThreeAddressInstruction::Modulo:
    out << "div $" << i.src1 << ", $" << i.src2 << std::endl;
    out << "\tmfhi $" << i.dest;
    break;
  case cs6300::ThreeAddressInstruction::Multiply:
    out << "mult $" << i.src1 << ", $" << i.src2 << std::endl;
    out << "\tmflo $" << i.dest;
    break;
  case cs6300::ThreeAddressInstruction::Not:
    out << "#TODO: not";
    break;
  case cs6300::ThreeAddressInstruction::Or:
    out << "or $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::ReadChar:
    out << "li $v0, 12" << std::endl;
    out << "\tsyscall" << std::endl;
    out << "\tmove $" << i.dest << ", $v0";
    break;
  case cs6300::ThreeAddressInstruction::ReadInt:
    out << "li $v0, 5" << std::endl;
    out << "\tsyscall" << std::endl;
    out << "\tmove $" << i.dest << ", $v0";
    break;
  case cs6300::ThreeAddressInstruction::RestoreFrame:
    out << "addi $sp, $sp, 8 # restore frame" << std::endl;
    out << "\tlw $ra, -8($sp)" << std::endl;
    out << "\tlw $fp, -4($sp)" << std::endl;
    out << "\taddi $sp, $sp, " << i.src1 << " #vars";
    break;
  case cs6300::ThreeAddressInstruction::Return:
    if (i.src1)
    {
      out << "move $v1, $" << i.src1 << std::endl;
      out << "\tjr $ra";
    }
    else if (i.dest)
      out << "move $" << i.dest << ", $v1";
    else
      out << "jr $ra";
    break;
  case cs6300::ThreeAddressInstruction::Stop:
    out << "li $v0, 10" << std::endl;
    out << "\tsyscall";
    break;
  case cs6300::ThreeAddressInstruction::StoreFrame:
    out << "addi $sp, $sp, " << -i.src1 << " # vars" << std::endl;
    out << "\tsw $fp, -4($sp) #store frame" << std::endl;
    out << "\tsw $ra, -8($sp)" << std::endl;
    out << "\taddi $sp, $sp, -8";
    break;
  case cs6300::ThreeAddressInstruction::StoreMemory:
    out << "sw $" << i.dest << ", " << i.src2 << "($" << i.src1
        << ") #store memory";
    break;
  case cs6300::ThreeAddressInstruction::StoreParameter:
    out << "sw $" << i.dest << ", " << i.src2
        << "($sp)   # storing a parameter";
    break;
  case cs6300::ThreeAddressInstruction::Subtract:
    out << "sub $" << i.dest << ", $" << i.src1 << ", $" << i.src2;
    break;
  case cs6300::ThreeAddressInstruction::UnaryMinus:
    out << "sub $" << i.dest << ", $zero"
        << ", $" << i.src1;
    break;
  case cs6300::ThreeAddressInstruction::WriteBool:
    out << "li $v0, 1" << std::endl;
    out << "\tsne $" << i.src1 << ", $" << i.src1 << ", $zero" << std::endl;
    out << "\tmove $a0, $" << i.src1 << std::endl;
    out << "\tsyscall";
    break;
  case cs6300::ThreeAddressInstruction::WriteChar:
    out << "li $v0, 11" << std::endl;
    out << "\tmove $a0, $" << i.src1 << std::endl;
    out << "\tsyscall";
    break;
  case cs6300::ThreeAddressInstruction::WriteInt:
    out << "li $v0, 1" << std::endl;
    out << "\tmove $a0, $" << i.src1 << std::endl;
    out << "\tsyscall    # writing an integer";
    break;
  case cs6300::ThreeAddressInstruction::WriteStr:
    out << "li $v0, 4" << std::endl;
    out << "\tla $a0, SL" << i.src1 << std::endl;
    out << "\tsyscall    # writing a string";
    break;
  default:
    std::cerr << "Unknown TAL address" << i.op << " " << i.src1 << " " << i.src2
              << std::endl;
  }
  out << std::endl;
  return out;
}
