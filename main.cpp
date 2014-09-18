#include "FrontEnd/FrontEnd.hpp"
#include "Optimizations/Optimizer.hpp"
#include "BackEnd/BackEnd.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
  try
  {
    std::string outFile = "out.asm";
    std::string inFile = "in.cpsl";

    if (argc < 2) return EXIT_FAILURE;
    if (argv[1] == std::string("-o"))
    {
      outFile = argv[2];
      inFile = argv[3];
    }
    else
    {
      inFile = argv[1];
    }

    auto program = cs6300::parseCPSL(inFile);
    auto optimized = cs6300::optimizer(program);
    auto intermediate =
      std::make_shared<cs6300::IntermediateRepresentationProgram>(optimized);
    intermediate->main = cs6300::optimizer(intermediate->main);
    for (auto&& f : intermediate->functions)
    {
      f.second = cs6300::optimizer(f.second);
    }
    cs6300::writeMIPS(intermediate, outFile);
  }
  catch (std::exception& e)
  {
    std::cout << "Error: " << e.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
