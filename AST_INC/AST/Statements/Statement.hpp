#ifndef CS6300_STATEMENTS_HPP
#define CS6300_STATEMENTS_HPP

#include "AST/BasicBlock.hpp"
#include "logger.h"

namespace cs6300
{
class Statement
{
public:
  Statement() : _id(-1), file(cpsl_log::getFile()), line(cpsl_log::getLine()) {}
  virtual ~Statement() = default;
  virtual cs6300::FlowGraph emit() = 0;
  virtual std::string ClassName() const = 0;
  std::vector<std::string> ASTLines() const;
  virtual std::string name() const;
  std::string id() const;

protected:
  virtual std::vector<std::string> _ASTLines() const = 0;
  std::string line;
  std::string file;

private:
  std::string node() const;
  mutable int _id;
};

FlowGraph emitList(std::vector<std::shared_ptr<Statement>> statements);

// Helpful wrapper for std::copy
std::vector<std::string>& copy(const std::vector<std::string>& src,
                               std::vector<std::string>& dst);

std::vector<std::string>& join(
  const std::vector<std::shared_ptr<cs6300::Statement>>&,
  std::vector<std::string>& dst,
  std::string id);
}

#endif
