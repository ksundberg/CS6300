#include <cstdio>
#include "FrontEnd.hpp"

extern FILE* yyin;
extern int yyparse();

std::shared_ptr<cs6300::Program> cs6300::parseCPSL(std::string filename) 
{
  FILE* inFile;
  inFile = fopen(filename.c_str(),"r");
  if(inFile == nullptr)
  {
    throw std::runtime_error("Could not open " + filename);
  }
  yyin = inFile;
  yyparse();
  return std::shared_ptr<cs6300::Program>();
}
