#include "StringTable.hpp"
#include <sstream>

namespace
{
std::string getLabel()
{
  static int count = 0;
  std::ostringstream oss;
  oss << "SL" << count++;
  return oss.str();
}
}

std::shared_ptr<cs6300::StringTable> cs6300::StringTable::m_instance;

std::string cs6300::StringTable::add(std::string val)
{
  auto iter = m_table.find(val);
  if (iter != m_table.end())
  {
    return iter->second;
  }
  auto label = getLabel();
  m_table[val] = label;
  return label;
}
