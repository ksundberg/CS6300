#ifndef CPSL_LOGGER_H
#define CPSL_LOGGER_H
#include "easylogging++.h"
#include <string>

namespace cpsl_log
{
  void init_log(int argc, char* argv[]);
  const char* getLine();
  const char* getFile();
}

#endif
