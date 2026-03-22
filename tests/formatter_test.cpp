#include "src/Logger/include/log_event.h"
#include "src/Queue/include/formatter.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <regex>
#include <string>
#include <vector>

TEST(FormatterTest, FormatsExpectedLayout) {
  auto event = std::make_unique<logger::LogEvent>("WARN", "hello payload", "my-app");

  queue::Formatter formatter;
  std::vector<uint8_t> buffer;
  formatter.LogFormatter(event, buffer);

  const std::string formatted(buffer.begin(), buffer.end());

  ASSERT_GT(formatted.size(), 20U);
  EXPECT_NE(formatted.find("[my-app]"), std::string::npos);
  EXPECT_NE(formatted.find("[WARN]"), std::string::npos);
  EXPECT_NE(formatted.find("hello payload"), std::string::npos);

  const std::regex prefix_re(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
  EXPECT_TRUE(std::regex_search(formatted, prefix_re));
}
