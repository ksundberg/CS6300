#include "logger.h"
#include "ProcessLog.hpp"

#include <boost/filesystem.hpp>
#include <string>
INITIALIZE_EASYLOGGINGPP

const char* cpsl_log::getLine()
{
  return strdup(std::to_string(ProcessLog::getInstance()->line()).c_str());
}

const char* cpsl_log::getFile()
{
  return strdup(boost::filesystem::basename(&ProcessLog::getInstance()->file()[0])
    .c_str());
}

const std::string defaultLogFile = "log/log.conf";
const std::string defaultLogConfig = R"(* GLOBAL:
    FORMAT               =  "%datetime{%H:%m:%s} [%fbase:%line|%asm_file:%asm_lineno] %msg"
    FILENAME             =  "cpsl.log"
    ENABLED              =  true
    TO_FILE              =  true
    TO_STANDARD_OUTPUT   =  false
    MILLISECONDS_WIDTH   =  6
    PERFORMANCE_TRACKING =  true
    MAX_LOG_FILE_SIZE    =  2097152 ## 2MB
    LOG_FLUSH_THRESHOLD  =  1 ## Flush after every log
* DEBUG:
    FORMAT               =  "%datetime{%H:%m:%s} [%fbase:%line|%asm_file:%asm_lineno] %msg"
* ERROR:
    TO_STANDARD_OUTPUT   =  true
* FATAL:
    TO_STANDARD_OUTPUT   =  true)";

// Load configuration from file and configure all loggers
void cpsl_log::init_log(int argc, char* argv[])
{
  START_EASYLOGGINGPP(argc, argv);

  el::Helpers::installCustomFormatSpecifier(
    el::CustomFormatSpecifier("%asm_lineno", cpsl_log::getLine));
  el::Helpers::installCustomFormatSpecifier(
    el::CustomFormatSpecifier("%asm_file", cpsl_log::getFile));

  el::Configurations conf;
  if (boost::filesystem::exists(defaultLogFile))
  {
    conf.parseFromFile(defaultLogFile);
  }
  else
  {
    conf.parseFromText(defaultLogConfig);
  }
  el::Loggers::setDefaultConfigurations(conf);
  el::Loggers::reconfigureAllLoggers(conf);
}
