#include "src/Logger/include/log_event.h"

#include <gtest/gtest.h>

#include <chrono>

TEST(LogEventTest, AccessorsReturnConstructedValues) {
  logger::LogEvent event("DEBUG", "payload-xyz", "app-xyz");

  EXPECT_EQ(event.GetLogLevel(), "DEBUG");
  EXPECT_EQ(event.GetPayload(), "payload-xyz");
  EXPECT_EQ(event.GetAppName(), "app-xyz");
  EXPECT_LE(event.GetTime(), logger::Clock::now());
}
