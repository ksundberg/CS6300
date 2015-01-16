#include <iostream>

// Unit testing header
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int argc, char* argv[])
{
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
