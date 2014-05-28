#include "ProcessLog.hpp"
#include <iostream>

std::shared_ptr<ProcessLog> ProcessLog::m_instance;

std::shared_ptr<ProcessLog> ProcessLog::getInstance()
{
  if(!m_instance)
  {
    m_instance = std::shared_ptr<ProcessLog>(new ProcessLog());
  }
  return m_instance;
}
void ProcessLog::nextLine()
{
  m_line++;
}

void ProcessLog::logError(std::string message)
{
  std::cout << "At line " << m_line << ": " << message << std::endl;
}

ProcessLog::ProcessLog()
:m_line(0)
{}


