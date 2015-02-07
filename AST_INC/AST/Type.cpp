#include <numeric>

#include "Type.hpp"
#include "logger.h"

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

void cs6300::RecordType::addMember(std::string id,
                                   std::shared_ptr<cs6300::Type> type)
{
  LOG(DEBUG) << "adding member " << id << " for type " << type->name();
  fields[id] = type;
  field_offset[id] = _offset;
  _offset += type->size();
}

std::shared_ptr<cs6300::Type> cs6300::RecordType::type(std::string id) const
{
  auto elem = fields.find(id);
  if (elem == fields.end())
  {
    LOG(FATAL) << "Unable to find type for member field " << id;
    return std::shared_ptr<cs6300::Type>();
  }

  return elem->second;
}

int cs6300::RecordType::offset(std::string id) const
{
  auto elem = field_offset.find(id);
  if (elem == field_offset.end())
  {
    LOG(FATAL) << "Unable to find offset for member field " << id;
    return -1;
  }

  return elem->second;
}

int cs6300::ArrayType::size()
{
  return (upperbound - lowerbound + 1) * baseType->size();
}
