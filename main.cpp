#include <cstdio>
#include <iostream>
#include <fstream>
#include "StringTable.hpp"
#include "RegisterPool.hpp"

extern FILE* yyin;
extern int yyparse();

std::ofstream cpslout;

int main(int/* argc*/, char * argv[])
{
  FILE * iFile;

  if(argv[1] == std::string("-o"))
  {
    cpslout.open(argv[2]);
    iFile = fopen(argv[3], "r");
  }
  else
  {
    cpslout.open("out.asm");
    iFile = fopen(argv[1], "r");
  }

  if(iFile == NULL)
  {
     std::cout << "Could not open input file " << argv[1] << std::endl;
     return 1;
  }

  yyin = iFile;

  cpslout << ".text" << std::endl
    << ".globl main" << std::endl
    << "main:"
    << "\tla $gp, GA" << std::endl
    << "\tori $fp, $sp, 0" << std::endl
    << "\tj program" << std::endl;

  yyparse();
  cpslout << ".data" << std::endl;
  StringTable::getInstance()->writeTable();
  cpslout << ".align 2" << std::endl;
  cpslout << "GA:" << std::endl;

  return 0;
}
