#include "Log.hpp"

#include <spdlog/spdlog.h>

#include <memory>

std::shared_ptr<spdlog::logger> Log::s_logger = spdlog::default_logger()->clone("URSA");

Log::Log()
{}

Log::~Log()
{}

void Log::setup(spdlog::level::level_enum log_level)
{
  s_logger->set_level(log_level);
  s_logger->set_pattern("[ %T ]--[%^ %L %$]--[ %n ]: %v");
}

std::shared_ptr<spdlog::logger> Log::getLogger()
{
  return s_logger;
}
