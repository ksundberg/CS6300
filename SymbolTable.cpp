#include "SymbolTable.hpp"
#include "Expressions.hpp"
#include "ProcessLog.hpp"
#include "RegisterPool.hpp"
#include "Type.hpp"

std::shared_ptr<SymbolTable> SymbolTable::m_instance;

SymbolTable::SymbolTable()
:m_tables()
  ,m_bool(new BoolType())
  ,m_char(new CharType())
  ,m_int(new IntType())
  ,m_string(new StringType())
{
  m_tables.emplace_back(true);
  addType("boolean",m_bool);
  addType("BOOLEAN",m_bool);
  addType("char",m_char);
  addType("CHAR",m_char);
  addType("integer",m_int);
  addType("INTEGER",m_int);
  addType("string",m_string);
  addType("STRING",m_string);
  //TODO add constants TRUE,true,FALSE,false
  m_tables.emplace_back(false);
}

std::shared_ptr<Type> SymbolTable::getBoolType()
{
  return m_bool;
}
std::shared_ptr<Type> SymbolTable::getCharType()
{
  return m_char;
}
std::shared_ptr<Type> SymbolTable::getIntType()
{
  return m_int;
}
std::shared_ptr<Type> SymbolTable::getStringType()
{
  return m_string;
}
void SymbolTable::addVariable(std::string id,std::shared_ptr<Type> type)
{
  m_tables.back().addVariable(id,type);
}

void SymbolTable::addVariable(IdList* list,std::shared_ptr<Type> type)
{
  if(list)
  {
    addVariable(list->id,type);
    addVariable(list->next,type);
  }
}
std::shared_ptr<MemoryLocation> SymbolTable::lookupVariable(std::string id)
{
  auto cur = m_tables.rbegin();
  while(cur!=m_tables.rend())
  {
    auto res = cur->lookupVariable(id);
    if(res) return res;
    ++cur;
  }
  ProcessLog::getInstance()->logError(id + " is not a variable");
  return std::shared_ptr<MemoryLocation>();
}
void SymbolTable::addType(std::string id,std::shared_ptr<Type> type)
{
  m_tables.back().addType(id,type);
}
std::shared_ptr<Type> SymbolTable::lookupType(std::string id)
{
  auto cur = m_tables.rbegin();
  while(cur!=m_tables.rend())
  {
    auto res = cur->lookupType(id);
    if(res)
    {
      return res;
    }
    ++cur;
  }
  ProcessLog::getInstance()->logError(id + " is not a type");
  return std::shared_ptr<Type>();
}
void SymbolTable::enterScope()
{
  m_tables.emplace_back(false);
}
void SymbolTable::exitScope()
{
  m_tables.pop_back();
}

bool SymbolTableLevel::isValid(std::string id)
{
  auto var = m_variables.find(id);
  if(var != m_variables.end())
  {
    ProcessLog::getInstance()->logError(id + " already defined as variable");
    return false;
  }
  auto type = m_types.find(id);
  if(type != m_types.end())
  {
    ProcessLog::getInstance()->logError(id + " already defined as type");
    return false;
  }
  return true;
}
void SymbolTableLevel::addVariable(std::string id,std::shared_ptr<Type> type)
{
  if(isValid(id))
  {
    auto loc = std::make_shared<MemoryLocation>();
    loc->offset = m_offset;
    loc->type = type;
    auto rp = RegisterPool::getInstance();
    if(m_globalScope)
    {
      loc->base = rp->getGp();
      m_offset += type->size();
    }
    else
    {
      loc->base = rp->getFp();
      m_offset -= type->size();
    }
    m_variables[id] = loc;
  }
}
std::shared_ptr<MemoryLocation> SymbolTableLevel::lookupVariable(std::string id)
{
  auto loc = m_variables.find(id);
  if(loc == m_variables.end()) return std::shared_ptr<MemoryLocation>();
  return loc->second;
}
void SymbolTableLevel::addType(std::string id,std::shared_ptr<Type> type)
{
  if(isValid(id))
  {
    m_types[id]=type;
  }
}
std::shared_ptr<Type> SymbolTableLevel::lookupType(std::string id)
{
  auto type = m_types.find(id);
  if(type == m_types.end()) return std::shared_ptr<Type>();
  return type->second;
}

