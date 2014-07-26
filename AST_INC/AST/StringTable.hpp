#ifndef STRING_TABLE_HPP 
#define STRING_TABLE_HPP 

#include <string>
#include <memory>
#include <map>

class StringTable
{
  public:
  static std::shared_ptr<StringTable> getInstance()
  {
    if(!m_instance) m_instance = std::shared_ptr<StringTable>(new StringTable());
    return m_instance;
  }
  std::string add(std::string);
  void writeTable();
  private:
  static std::shared_ptr<StringTable> m_instance;
  std::map<std::string,std::string> m_table;
};
#endif
