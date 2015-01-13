#ifndef PROCESS_LOG_HPP
#define PROCESS_LOG_HPP

#include <memory>
#include <string>

class ProcessLog
{
public:
  static std::shared_ptr<ProcessLog> getInstance();

  // increment file line
  void nextLine();
  void set_infile(std::string f) { in_file = f; }

  // get file line
  int line() { return m_line; }
  std::string file() { return in_file; }

private:
  ProcessLog();
  size_t m_line;
  std::string in_file;
  static std::shared_ptr<ProcessLog> m_instance;
};

#endif
