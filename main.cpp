#include <cstdio>
#include <iostream>

extern FILE* yyin;
extern int yyparse();

int main(int argc, char * argv[])
{
  FILE * iFile;

  iFile = fopen(argv[1], "r");

  if(iFile == NULL)
  {
     std::cout << "Could not open input file " << argv[1] << std::endl;
     return 1;
  }

  yyin = iFile;


  yyparse();

  return 0;
}
