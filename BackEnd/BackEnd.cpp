#include "BackEnd.hpp"
#include <fstream>

void cs6300::writeMIPS(std::shared_ptr<Program>, std::string filename)
{
  std::ofstream fout(filename);
  fout << ".text" << std::endl << ".globl main" << std::endl << "main:"
       << "\tla $gp, GA" << std::endl << "\tori $fp, $sp, 0" << std::endl
       << "\tj program" << std::endl;

  fout << ".data" << std::endl;
  //StringTable::getInstance()->writeTable();
  fout << ".align 2" << std::endl;
  fout << "GA:" << std::endl;
}
