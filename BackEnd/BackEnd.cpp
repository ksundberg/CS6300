#include "BackEnd.hpp"
#include "AST/Program.hpp"
#include <fstream>

namespace{

void emitMIPS(std::pair<std::shared_ptr<cs6300::BasicBlock>,
                         std::shared_ptr<cs6300::BasicBlock>> cfg,
               std::ofstream& fout)
  {
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
