#include "../../Queue/include/formatter.h"

namespace queue {


void Formatter::LogFormatter(const std::unique_ptr<logger::LogEvent> &log_event,
                  std::vector<uint8_t> &byte_buf) {
   Time time = log_event->GetTime();
   time_str_ = timePointToString(time);
   app_name_ = log_event->GetAppName();
   log_level_ = log_event->GetLogLevel();
   payload_ = log_event->GetPayload();
   formatted_log_ = time_str_ + " " + "[" + app_name_ + "]" + " " +
                              "[" + log_level_ + "]" + " " + payload_ + "\n";
  byte_buf = std::vector<uint8_t>(formatted_log_.begin(), formatted_log_.end());
}
} // namespace queue