#include "BackEnd.hpp"
#include "AST/Program.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include "AST/StringTable.hpp"
#include "Allocation.hpp"
#include <fstream>

namespace
{

std::set<std::shared_ptr<cs6300::BasicBlock>> seenBlocks;

void emitMIPS(std::shared_ptr<cs6300::BasicBlock> block, std::ofstream& fout)
{

  if (!block || (seenBlocks.find(block) != seenBlocks.end())) return;
  seenBlocks.insert(block);
  fout << block->getLabel() << ":\t" << std::endl;
  for (auto&& i : block->instructions)
  {
    fout << i;
  }
  if (block->branchTo)
  {
    fout << "\tbne $" << block->branchOn << ", $zero, "
         << block->branchTo->getLabel() << std::endl;
  }
  if (block->jumpTo)
  {
    fout << "\tj " << block->jumpTo->getLabel() << std::endl;
  }

  emitMIPS(block->branchTo, fout);
  emitMIPS(block->jumpTo, fout);
}
void emitMIPS(std::pair<std::shared_ptr<cs6300::BasicBlock>,
                        std::shared_ptr<cs6300::BasicBlock>> cfg,
              std::ofstream& fout)
{
  seenBlocks.clear();
  emitMIPS(cfg.first, fout);
}
}

void writeStringTable(std::ofstream& fout)
{
  auto string_table = cs6300::StringTable::instance()->getTable();
  for (auto s : string_table)
  {
    fout << "SL" << s.second << ": \t.asciiz " << s.first << std::endl;
  }
}

void pushRegister(std::string srcReg, std::ofstream& fout)
{
    fout << "\tsw " << srcReg << ", ($sp) # Push the " << srcReg << " register" <<std::endl;
    fout << "\taddi $sp, $sp, -4" << std::endl;
}
void  popRegister(std::string dstReg, std::ofstream& fout)
{
   fout << "\taddi $sp, $sp, 4 # Pop the " << dstReg << " register" << std::endl;
    fout << "\tlw " << dstReg << ", ($sp)" <<std::endl;
}


void emitFunctionProlog(std::ofstream& fout)
{
    pushRegister("$ra", fout);
    pushRegister("$fp", fout);
    fout << "\tmove $fp, $sp      # Make the new stack pointer our current frame pointer" << std::endl;
}

void emitFunctionEpilog(std::ofstream& fout)
{
    fout << "\tmove $sp, $fp" << std::endl;
    popRegister("$fp", fout);
    popRegister("$ra", fout);
    fout << "\tjr $ra" << std::endl;
}


void cs6300::writeMIPS(
  std::shared_ptr<IntermediateRepresentationProgram> program,
  std::string filename)
{
  locRegAlloc(program->main);
  std::ofstream fout(filename);
  fout << ".text" << std::endl << ".globl main" << std::endl << "main:"
       << "\tla $gp, GA" << std::endl << "\tori $fp, $sp, 0" << std::endl;

  emitMIPS(program->main, fout);

  for (auto&& f : program->functions)
  {
    fout << "F" << f.first.getLabel() << ":" << std::endl;
    locRegAlloc(f.second);
      emitFunctionProlog(fout);
    emitMIPS(f.second, fout);
      emitFunctionEpilog(fout);
  }

  fout << ".data" << std::endl;
  writeStringTable(fout);

  fout << ".align 2" << std::endl;
  fout << "GA:" << std::endl;
}
