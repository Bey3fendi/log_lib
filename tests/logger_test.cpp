#include "src/Logger/include/logger.h"
#include "src/Sink/include/file_sink.h"

#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

namespace {

std::string ReadFile(const std::filesystem::path &path) {
  std::ifstream in(path);
  std::string all;
  std::string line;
  while (std::getline(in, line)) {
    all += line;
    all += '\n';
  }
  return all;
}

TEST(LoggerTest, ShutdownReturnsFalseWhenWorkerNeverStarted) {
  auto &logger = logger::SLogger::getInstance();
  logger.init(R"({"app_name":"logger-test","level":"INFO","pid":"111"})");

  EXPECT_FALSE(logger.shutdown());
}

TEST(LoggerTest, StartAndImmediateShutdownReturnsTrue) {
  auto &logger = logger::SLogger::getInstance();
  logger.init(R"({"app_name":"logger-test","level":"INFO","pid":"150"})");
  logger.start();

  EXPECT_TRUE(logger.shutdown());
}

TEST(LoggerTest, StartLogAndShutdownReturnsTrue) {
  const std::filesystem::path path = sink::FileSink::DefaultLogPath();
  std::error_code ec;
  std::filesystem::remove(path, ec);

  auto &logger = logger::SLogger::getInstance();
  logger.init(R"({"app_name":"logger-test","level":"INFO","pid":"222"})");
  logger.start();

  logger.log("INFO", "logger branch payload");
  logger.log("logger branch payload");
  logger.log("INFO", "logger branch payload");

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  EXPECT_TRUE(logger.shutdown());

  const std::string content = ReadFile(path);
  EXPECT_NE(content.find("[logger-test]"), std::string::npos);
  EXPECT_NE(content.find("[INFO]"), std::string::npos);
  EXPECT_NE(content.find("logger branch payload"), std::string::npos);
}

} // namespace
