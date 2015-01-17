#include "logger.h"
#include "ProcessLog.hpp"

#include <boost/filesystem.hpp>
#include <string>

const char* cpsl_log::getLine() {
    return std::to_string(ProcessLog::getInstance()->line()).c_str();
}

const char* cpsl_log::getFile() {
    return boost::filesystem::basename(&ProcessLog::getInstance()->file()[0]).c_str();
}

// Load configuration from file and configure all loggers
void cpsl_log::init_log(int argc, char* argv[])
{
  START_EASYLOGGINGPP(argc, argv);

  el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%asm_lineno", cpsl_log::getLine));
  el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%asm_file", cpsl_log::getFile));

  el::Configurations conf("../log/log.conf");
  el::Loggers::reconfigureAllLoggers(conf);
}
