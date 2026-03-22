#pragma once
#include "../../Logger/include/log_event.h"
#include <cstdint>
#include <iomanip>
#include <memory>
#include <sys/types.h>
#include <vector>

namespace queue {
using Clock = std::chrono::high_resolution_clock;
using Time = Clock::time_point;

class Formatter { //@TODO: class need to implemented with proper methods cpp
                  // file also included.
public:
  Formatter() = default;

  void LogFormatter(const std::unique_ptr<logger::LogEvent> &log_event,
                    std::vector<uint8_t> &byte_buf);

private:
  std::string timePointToString(std::chrono::system_clock::time_point &tp) {
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&tt); // Use std::gmtime for UTC

    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); // Format string

    return ss.str();
  }

  std::string time_str_{};
  std::string app_name_{}; 
  std::string log_level_{}; 
  std::string payload_{};
  std::string formatted_log_{};
};

} // namespace queue