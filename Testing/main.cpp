#define CATCH_CONFIG_RUNNER
#include "logger.h"
#include "catch.hpp"

int main(int argc, char* argv[])
{
  cpsl_log::init_log(argc, argv);
  try
  {
    return Catch::Session().run(argc, argv);
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what();
    return EXIT_FAILURE;
  }
  catch (...)
  {
    std::cerr << "Unknown Error: " << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
