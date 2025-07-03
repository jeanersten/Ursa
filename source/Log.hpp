#pragma once

#include <spdlog/spdlog.h>

#include <memory>

#define LOG_LEVEL_TRACE    spdlog::level::trace
#define LOG_LEVEL_DEBUG    spdlog::level::debug 
#define LOG_LEVEL_INFO     spdlog::level::info
#define LOG_LEVEL_WARN     spdlog::level::warn
#define LOG_LEVEL_ERROR    spdlog::level::err
#define LOG_LEVEL_CRITICAL spdlog::level::critical

class Log
{
private:
  static std::shared_ptr<spdlog::logger> s_logger;

  Log();
  ~Log();

public:
  static void setup(spdlog::level::level_enum log_level);
  static std::shared_ptr<spdlog::logger> getLogger();
};

#define LOG_TRACE(msg, ...)    Log::getLogger()->trace(msg, __VA_ARGS__)
#define LOG_DEBUG(msg, ...)    Log::getLogger()->debug(msg, __VA_ARGS__)
#define LOG_INFO(msg, ...)     Log::getLogger()->info(msg, __VA_ARGS__)
#define LOG_WARN(msg, ...)     Log::getLogger()->warn(msg, __VA_ARGS__)
#define LOG_ERROR(msg, ...)    Log::getLogger()->error(msg, __VA_ARGS__)
#define LOG_CRITICAL(msg, ...) Log::getLogger()->critical(msg, __VA_ARGS__)
