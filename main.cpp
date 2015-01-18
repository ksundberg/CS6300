#include <iostream>
#include <string>
#include <unistd.h>

#include "BackEnd/BackEnd.hpp"
#include "FrontEnd/FrontEnd.hpp"
#include "Optimizations/FlowGraph.h"
#include "Optimizations/Optimizer.hpp"
#include "BackEnd/BackEnd.hpp"
#include "ProcessLog.hpp"

// Logging header
#include "logger.h"
INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
  cpsl_log::init_log(argc, argv);

  try
  {
    int opt;
    bool emitFlowGraph = false;
    bool emitOptimizedFlowGraph = false;
    bool emitAST = false;
    std::string outFile = "";
    std::string inFile = "";

    while ((opt = getopt (argc, argv, "aFfo:i:")) != -1)
    {
      switch (opt)
      {
        case 'f':
          emitFlowGraph = true;
          break;
        case 'F':
          emitOptimizedFlowGraph = true;
          break;
        case 'a':
          emitAST = true;
          break;
        case 'o':
          outFile = optarg;
          break;
        case 'i':
          inFile = optarg;
          break;
        case '?':
          std::cerr << "Unknown option character '" << char(optopt) << "'" << std::endl;
          return EXIT_FAILURE;
        default:
          // All options must precede filenames
          break;
      }
    }

    if(emitFlowGraph && emitOptimizedFlowGraph)
    {
      std::cerr << "Warning: generating multiple flow graphs" << std::endl;
    }

    if(inFile == "")
    {
      if(argc > 1 && argv[argc-1][0] != '-')
      {
        inFile = argv[argc-1];
      }
      else
      {
        inFile = "in.cpsl";
      }
    }

    if(outFile == "") outFile = "out.asm";
    LOG(INFO) << "Compiling " << inFile << " to " << outFile;

    ProcessLog::getInstance()->set_infile(inFile);
    LOG(INFO) << "Compiling " << inFile << " to " << outFile;

    auto program = cs6300::parseCPSL(inFile);

    if (emitAST)
    {
      std::string ast;
      for(auto&s : program->main)
      {
        //TODO: Find out why some statements are NULL
        if(!s) continue;

        ast += "main -> " + s->id() + "\n";
        for(auto& edge : s->ASTLines())
        {
            ast += edge + "\n";
        }
      }
      for (auto&&f : program->functions)
      {
        for(auto&s : f.second->body)
        {
          if(!s) continue;
          ast += f.first.name + " -> " + s->id() + "\n";
          for(auto& edge : s->ASTLines())
          {
            ast += edge + "\n";
          }
        }
      }
      std::cout << "digraph G {\n" << "rank=same\n" << ast << "}" << std::endl;
    }

    auto optimized = cs6300::optimizer(program);
    auto intermediate =
      std::make_shared<cs6300::IntermediateRepresentationProgram>(optimized);

    /* Print out flowgraph */
    if(emitFlowGraph)
      std::cout << cs6300::flowGraphDot(intermediate->main);

    intermediate->main = cs6300::optimizer(intermediate->main);
    for (auto&& f : intermediate->functions)
    {
      f.second = cs6300::optimizer(f.second);
    }

    /* Print out optimized flowgraph */
    if(emitOptimizedFlowGraph)
      std::cout << cs6300::flowGraphDot(intermediate->main);

    cs6300::writeMIPS(intermediate, outFile);
  }
  catch (std::exception& e)
  {
    LOG(ERROR) << "Error: " << e.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
