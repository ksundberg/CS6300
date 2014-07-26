#ifndef CS6300_FRONT_END_HPP
#define CS6300_FRONT_END_HPP

#include <memory>

#include "AST/Program.hpp"

namespace cs6300
{
  /*Returns the Abstract Syntax Tree that is the result of parsing filename*/
  std::shared_ptr<Program> parseCPSL(std::string filename);
}

#endif
