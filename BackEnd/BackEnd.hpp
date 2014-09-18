#ifndef CS6300_BACK_END_HPP
#define CS6300_BACK_END_HPP

#include <memory>
namespace cs6300
{
  class IntermediateRepresentationProgram;
  void writeMIPS(std::shared_ptr<IntermediateRepresentationProgram>,std::string filename);
}
#endif
