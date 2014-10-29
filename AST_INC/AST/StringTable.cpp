#include "StringTable.hpp"
#include <sstream>

namespace
{
int getLabel()
{
  static int count = 0;
  return count++;
}
}

std::shared_ptr<cs6300::StringTable> cs6300::StringTable::m_instance;

int cs6300::StringTable::add(std::string val)
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

std::map<std::string, int> cs6300::StringTable::getTable()
{
  return m_table;
}
