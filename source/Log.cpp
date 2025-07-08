#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

std::shared_ptr<spdlog::sinks::stderr_color_sink_mt> Log::s_stderr_console = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
std::shared_ptr<spdlog::logger> Log::s_logger = std::make_shared<spdlog::logger>("URSA");

Log::Log()
{}

Log::~Log()
{}

void Log::setup(spdlog::level::level_enum log_level)
{
  s_logger->sinks().push_back(s_stderr_console);
  s_logger->set_level(log_level);
  s_logger->set_pattern("[ %T ]--[%^ %L %$]--[ %n ]: %v");
}

std::shared_ptr<spdlog::logger> Log::getLogger()
{
  return s_logger;
}
