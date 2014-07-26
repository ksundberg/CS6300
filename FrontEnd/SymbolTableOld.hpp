#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>

class Type;
class MemoryLocation;

class IdList
{
  public:
  std::string id;
  IdList* next;
  IdList(std::string id,IdList* next):id(id),next(next){}
  ~IdList(){delete(next);}
};

class SymbolTableLevel
{
  public:
    SymbolTableLevel(bool isGlobal)
      :m_offset(0)
       ,m_globalScope(isGlobal)
  {}
  void addVariable(std::string id,std::shared_ptr<Type> type);
  std::shared_ptr<MemoryLocation> lookupVariable(std::string id);
  void addType(std::string id,std::shared_ptr<Type> type);
  std::shared_ptr<Type> lookupType(std::string id);
  private:
  bool isValid(std::string id);
  std::map<std::string,std::shared_ptr<MemoryLocation>> m_variables;
  std::map<std::string,std::shared_ptr<Type>> m_types;
  int m_offset;
  bool m_globalScope;

};

class SymbolTable
{
  public:
  static std::shared_ptr<SymbolTable> getInstance()
  {
    if(!m_instance) m_instance = std::shared_ptr<SymbolTable>(new SymbolTable());
    return m_instance;
  }
  std::shared_ptr<Type> getBoolType();
  std::shared_ptr<Type> getCharType();
  std::shared_ptr<Type> getIntType();
  std::shared_ptr<Type> getStringType();
  void addVariable(std::string id,std::shared_ptr<Type> type);
  void addVariable(IdList* list,std::shared_ptr<Type> type);
  std::shared_ptr<MemoryLocation> lookupVariable(std::string id);
  void addType(std::string id,std::shared_ptr<Type> type);
  std::shared_ptr<Type> lookupType(std::string id);
  void enterScope();
  void exitScope();
  private:
  SymbolTable();
  static std::shared_ptr<SymbolTable> m_instance;
  std::vector<SymbolTableLevel> m_tables;
  std::shared_ptr<Type> m_bool;
  std::shared_ptr<Type> m_char;
  std::shared_ptr<Type> m_int;
  std::shared_ptr<Type> m_string;
};
#endif
