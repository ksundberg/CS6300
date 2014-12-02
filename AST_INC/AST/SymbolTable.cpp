#include <string>
#include "SymbolTable.hpp"
#include "Expressions/Expression.hpp"
#include "Type.hpp"
#include <map>
#include <sstream>
#include "Symbol.hpp"

std::shared_ptr<cs6300::Type> cs6300::SymbolTable::lookupType(std::string id)
{
  auto found = m_types.find(id);
  if (found != m_types.end()) return found->second;
  if (m_parent) return m_parent->lookupType(id);
  return nullptr;
}

std::shared_ptr<cs6300::Symbol> cs6300::SymbolTable::lookupVariable(
  std::string id)
{
  auto found = m_variables.find(id);
  if (found != m_variables.end()) return found->second;
  if (m_parent) return m_parent->lookupVariable(id);
  return nullptr;
}

std::shared_ptr<cs6300::Symbol> cs6300::SymbolTable::lookupRecordVariable(
  std::string id, std::shared_ptr<Type> type)
{
  std::stringstream idStrm;
  idStrm << id << type;
  auto found = m_variables.find(idStrm.str());
  if (found != m_variables.end()) return found->second;
  if (m_parent) return m_parent->lookupRecordVariable(id, type);
  return nullptr;
}

std::shared_ptr<cs6300::Expression> cs6300::SymbolTable::lookupConstant(
  std::string id)
{
  auto found = m_consts.find(id);
  if (found != m_consts.end()) return found->second;
  if (m_parent) return m_parent->lookupConstant(id);
  return nullptr;
}

void cs6300::SymbolTable::addConstant(std::string id,
                                      std::shared_ptr<Expression> val)
{
  auto found = m_consts.find(id);
  if (found == m_consts.end())
  {
    m_consts[id] = val;
  }
}

void cs6300::SymbolTable::addType(std::string id, std::shared_ptr<Type> type)
{
  auto found = m_types.find(id);
  if (found == m_types.end())
  {
    m_types[id] = type;
  }
}

void cs6300::SymbolTable::addVariable(std::string id,
                                      std::shared_ptr<Type> type)
{
  auto found = m_variables.find(id);
  if (found == m_variables.end())
  {
      m_variables[id] = std::make_shared<Symbol>(id, type,  m_memory_offset , m_memorylocation);
  }
 m_memory_offset += type->size();
}

void cs6300::SymbolTable::addRecordVariable(std::string id,
                                            std::shared_ptr<Type> type)
{
  std::stringstream idStrm;
  idStrm << id << type;
  auto found = m_variables.find(idStrm.str());
  if (found == m_variables.end())
  {
      m_variables[idStrm.str()] = std::make_shared<Symbol>(id, type,  m_memory_offset , m_memorylocation);
  }
 m_memory_offset += type->size();

}

void cs6300::SymbolTable::addParameter(std::string id,
                                      std::shared_ptr<Type> type)
{
    auto found = m_variables.find(id);
    if (found == m_variables.end())
    {
        m_variables[id] = std::make_shared<Symbol>(id, type,  m_param_offset , FRAME);
    }
    m_param_offset += type->size();
}

