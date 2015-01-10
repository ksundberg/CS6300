#ifndef PROCESS_LOG_HPP
#define PROCESS_LOG_HPP

#include <memory>
#include <string>

class ProcessLog
{
public:
  static std::shared_ptr<ProcessLog> getInstance();
  void nextLine();
  void logError(std::string message);

private:
  ProcessLog();
  size_t m_line;
  static std::shared_ptr<ProcessLog> m_instance;
};

#endif
