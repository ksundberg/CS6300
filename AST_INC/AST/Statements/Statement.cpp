#include "Statement.hpp"

cs6300::FlowGraph cs6300::emitList(
  std::vector<std::shared_ptr<Statement>> statements)
{
  auto entry = std::make_shared<BasicBlock>();
  auto current = entry;
  for (auto&& s : statements)
  {
    if (s)
    {
      auto subgraph = s->emit();
      current->jumpTo = subgraph.first;
      current = subgraph.second;
    }
  }
  return std::make_pair(entry, current);
}

std::vector<std::string>& cs6300::copy(const std::vector<std::string>& src,
                                       std::vector<std::string>& dst)
{
  std::copy(src.begin(), src.end(), std::back_inserter(dst));
  return dst;
}

std::string cs6300::Statement::id() const
{
  static int id = 0;
  if (_id == -1) _id = ++id;
  return ClassName() + std::to_string(_id);
}

std::string cs6300::Statement::node() const
{
  return id() + " [label=" + name() + "]";
}

std::string cs6300::Statement::name() const
{
  return ClassName();
}

std::vector<std::string> cs6300::Statement::ASTLines() const
{
  std::vector<std::string> ast = {node()};

  for (auto& e : _ASTLines())
    ast.emplace_back(e);

  return ast;
}

std::vector<std::string>& cs6300::join(
  const std::vector<std::shared_ptr<cs6300::Statement>>& statements,
  std::vector<std::string>& dst,
  std::string id)
{
    int num = 0;
  for (auto& s : statements)
  {
    if (!s) continue;
    dst.emplace_back(id + " -> " + s->id() + "[label=" + std::to_string(++num) + "]");
    copy(s->ASTLines(), dst);
  }
  return dst;
}
