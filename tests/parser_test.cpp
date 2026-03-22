#include "src/utils/include/json_parser.h"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

TEST(JsonParserTest, ParsesValidConfig) {
  const std::string input =
      R"({"app_name":"my-app","level":"INFO","pid":"123"})";

  auto [app_name, level, pid] = log_config_parser(input);
  EXPECT_EQ(app_name, "my-app");
  EXPECT_EQ(level, "INFO");
  EXPECT_EQ(pid, "123");
}

TEST(JsonParserTest, ParseErrorBranchReturnsEmptyTupleAndLogs) {
  std::ostringstream err_capture;
  auto *old_err = std::cerr.rdbuf(err_capture.rdbuf());

  auto [app_name, level, pid] = log_config_parser("{bad json");

  std::cerr.rdbuf(old_err);

  EXPECT_EQ(app_name, "");
  EXPECT_EQ(level, "");
  EXPECT_EQ(pid, "");
  EXPECT_NE(err_capture.str().find("JSON parse error"), std::string::npos);
}

TEST(JsonParserTest, AccessErrorBranchReturnsEmptyTupleAndLogs) {
  std::ostringstream err_capture;
  auto *old_err = std::cerr.rdbuf(err_capture.rdbuf());

  auto [app_name, level, pid] =
      log_config_parser(R"({"app_name":"my-app","level":"INFO"})");

  std::cerr.rdbuf(old_err);

  EXPECT_EQ(app_name, "");
  EXPECT_EQ(level, "");
  EXPECT_EQ(pid, "");
  EXPECT_NE(err_capture.str().find("JSON access/type error"), std::string::npos);
}
