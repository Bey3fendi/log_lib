#include "../include/log_event.h"

namespace logger {
std::string LogEvent::GetLogLevel() { return log_level_; }

std::string LogEvent::GetPayload() { return payload_; }

std::string LogEvent::GetAppName() { return app_name_; }

Time LogEvent::GetTime() { return time_; };
} // namespace logger