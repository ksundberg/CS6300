#include <fstream>
#include "BackEnd.hpp"
#include "AST/Program.hpp"
#include "AST/ThreeAddressInstruction.hpp"
#include <set>

namespace{
    
std::set<std::shared_ptr<cs6300::BasicBlock>> seenBlocks ;

void emitMIPS(std::shared_ptr<cs6300::BasicBlock> block, std::ofstream &fout)
{
    
  if (!block || (seenBlocks.find(block) != seenBlocks.end()))
    return;
  seenBlocks.insert(block);
  fout << block->getLabel() << ":\t";
  for (auto &&i : block->instructions)
  {
    fout << i;
  }
  if (block->branchTo)
  {
    fout << "\tbeq $" << block->branchOn << ", " << block->branchTo->getLabel()
         << std::endl;
  }
  if (block->jumpTo)
  {
    fout << "\tj " << block->jumpTo->getLabel() << std::endl;
  }

  emitMIPS(block->branchTo,fout);
  emitMIPS(block->jumpTo,fout);
}
void emitMIPS(std::pair<std::shared_ptr<cs6300::BasicBlock>,
                         std::shared_ptr<cs6300::BasicBlock>> cfg,
               std::ofstream& fout)
  {
    seenBlocks.clear();
    emitMIPS(cfg.first,fout);
  }
}
void cs6300::writeMIPS(std::shared_ptr<IntermediateRepresentationProgram> program, std::string filename)
{
  std::ofstream fout(filename);
  fout << ".text" << std::endl << ".globl main" << std::endl << "main:"
       << "\tla $gp, GA" << std::endl << "\tori $fp, $sp, 0" << std::endl;
  
  emitMIPS(program->main,fout);

  for(auto&& f:program->functions)
  {
    emitMIPS(f.second,fout);
  }

  fout << ".data" << std::endl;
  //StringTable::getInstance()->writeTable();
  fout << ".align 2" << std::endl;
  fout << "GA:" << std::endl;
}
