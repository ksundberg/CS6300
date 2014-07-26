#ifndef CS6300_BACK_END_HPP
#define CS6300_BACK_END_HPP

#include <memory>
namespace cs6300
{
  class Program;
  void writeMIPS(std::shared_ptr<Program>,std::string filename);
}
#endif
