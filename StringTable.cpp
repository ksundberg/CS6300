#include "StringTable.hpp"
#include <sstream>
#include <fstream>

extern std::ofstream cpslout;

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

std::shared_ptr<StringTable> StringTable::m_instance;

std::string StringTable::add(std::string val)
{
  auto iter = m_table.find(val);
  if(iter != m_table.end())
  {
    return iter->second;
  }
  auto label = getLabel();
  m_table[val] = label;
  return label;
}

void StringTable::writeTable()
{
  for(auto & e:m_table)
  {
    cpslout << e.second << ":" << std::endl
            << ".asciiz " << e.first << std::endl;
  }
}
