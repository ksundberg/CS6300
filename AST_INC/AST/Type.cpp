#include "Type.hpp"
#include <numeric>

std::shared_ptr<cs6300::Type> cs6300::BuiltInType::m_char;
std::shared_ptr<cs6300::Type> cs6300::BuiltInType::m_int;
std::shared_ptr<cs6300::Type> cs6300::BuiltInType::m_bool;
std::shared_ptr<cs6300::Type> cs6300::BuiltInType::m_str;

int cs6300::RecordType::size()
{
  return std::accumulate(
    std::begin(fields),
    std::end(fields),
    0,
    [](int cur, std::pair<std::string, std::shared_ptr<Type>> element)
    {
      return element.second->size() + cur;
    });
}

int cs6300::ArrayType::size()
{
  return (upperbound - lowerbound) * baseType->size();
}
