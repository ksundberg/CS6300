#include "ProcessLog.hpp"
#include <iostream>

std::shared_ptr<ProcessLog> ProcessLog::m_instance;

std::shared_ptr<ProcessLog> ProcessLog::getInstance()
{
  if (!m_instance)
  {
    m_instance = std::shared_ptr<ProcessLog>(new ProcessLog());
  }
  return m_instance;
}
void ProcessLog::nextLine()
{
  m_line++;
}

ProcessLog::ProcessLog() : m_line(1)
{
}
