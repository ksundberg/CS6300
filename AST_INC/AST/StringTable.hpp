#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

#include <string>
#include <memory>
#include <map>

namespace cs6300
{
class StringTable
{
public:
  static std::shared_ptr<StringTable> instance()
  {
    if (!m_instance)
      m_instance = std::shared_ptr<StringTable>(new StringTable());
    return m_instance;
  }
  int add(std::string);
  std::map<std::string, int> getTable();

private:
  static std::shared_ptr<StringTable> m_instance;
  std::map<std::string, int> m_table;
};
}
#endif
