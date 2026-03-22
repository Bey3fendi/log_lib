#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
namespace logger {
using Clock = std::chrono::high_resolution_clock;
using Time = Clock::time_point;
class LogEvent {

public:
  LogEvent(std::string_view log_level, std::string_view payload,
           std::string_view app_name)
      : time_(Clock::now()), log_level_(log_level), payload_(payload),
        app_name_(app_name) {}

  std::string GetLogLevel();

  std::string GetPayload();

  std::string GetAppName();

  Clock::time_point GetTime();

private:
  Time time_;
  std::string log_level_;
  std::string payload_;
  std::string app_name_;
};
} // namespace logger