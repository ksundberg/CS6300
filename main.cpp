#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "logger.h"
INITIALIZE_EASYLOGGINGPP

#include "BackEnd/BackEnd.hpp"
#include "FrontEnd/FrontEnd.hpp"
#include "Optimizations/FlowGraph.h"
#include "Optimizations/Optimizer.hpp"
#include "BackEnd/BackEnd.hpp"
#include "ProcessLog.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  cpsl_log::init_log(argc, argv);

  try
  {
    bool emitFlowGraph = false;
    bool emitOptimizedFlowGraph = false;
    std::string outFile = "";
    std::string inFile = "";

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
      "input,i", po::value<std::string>(), "input cpsl file")(
      "output,o", po::value<std::string>(), "output asm file")(
      "flowgraph,f", "output flowgraph")(
      "flowgraph-optimized,F", "output optimized flowgraph");

    po::positional_options_description positionalOptions;
    positionalOptions.add("input", 1);
    positionalOptions.add("output", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                .options(desc)
                .positional(positionalOptions)
                .run(),
              vm);
    po::notify(vm);

    if (vm.count("help"))
    {
      std::cout << desc << "\n";
      return EXIT_SUCCESS;
    }

    if (vm.count("flowgraph"))
    {
      emitFlowGraph = true;
    }
    if (vm.count("flowgraph-optimized"))
    {
      emitOptimizedFlowGraph = true;
    }
    if (vm.count("output"))
    {
      outFile = vm["output"].as<std::string>();
    }
    else
    {
      outFile = "out.asm";
    }
    if (vm.count("input"))
    {
      inFile = vm["input"].as<std::string>();
    }
    else
    {
      inFile = "in.cpsl";
    }

    if (emitFlowGraph && emitOptimizedFlowGraph)
    {
      std::cerr << "Warning: generating multiple flow graphs" << std::endl;
    }

    LOG(INFO) << "Compiling " << inFile << " to " << outFile;

    ProcessLog::getInstance()->set_infile(inFile);
    LOG(INFO) << "Compiling " << inFile << " to " << outFile;

    auto program = cs6300::parseCPSL(inFile);
    auto optimized = cs6300::optimizer(program);
    auto intermediate =
      std::make_shared<cs6300::IntermediateRepresentationProgram>(optimized);

    /* Print out flowgraph */
    if (emitFlowGraph) std::cout << cs6300::flowGraphDot(intermediate->main);

    intermediate->main = cs6300::optimizer(intermediate->main);
    for (auto&& f : intermediate->functions)
    {
      f.second = cs6300::optimizer(f.second);
    }

    /* Print out optimized flowgraph */
    if (emitOptimizedFlowGraph)
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
