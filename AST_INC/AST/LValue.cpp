#include "LValue.hpp"

std::vector<std::string>& cs6300::join(
  const std::shared_ptr<cs6300::LValue>& lval,
  std::vector<std::string>& dst,
  std::string id)
{
  dst.emplace_back(lval->node());
  dst.emplace_back(id + " -> " + lval->id());
  return copy(lval->ASTDot(), dst);
}

std::vector<std::string>& cs6300::join(
  const std::vector<std::shared_ptr<cs6300::LValue>>& lvals,
  std::vector<std::string>& dst,
  std::string id)
{
  for (auto& lval : lvals)
    join(lval, dst, id);
  return dst;
}
