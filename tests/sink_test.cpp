#include "src/Sink/include/console_sink.h"
#include "src/Sink/include/file_sink.h"
#include "src/Sink/include/sink_manager.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace {

class MemorySink final : public sink::ISink {
public:
  void Write(const std::vector<uint8_t> &bytes) override {
    data_.append(bytes.begin(), bytes.end());
  }

  void Flush() override { flushed_ = true; }

  const std::string &Data() const { return data_; }
  bool Flushed() const { return flushed_; }

private:
  std::string data_;
  bool flushed_{false};
};

std::vector<uint8_t> ToBytes(const std::string &s) {
  return std::vector<uint8_t>(s.begin(), s.end());
}

TEST(ConsoleSinkTest, EmptyInputWritesNothing) {
  sink::ConsoleSink sink;

  std::ostringstream capture;
  auto *old_buf = std::cout.rdbuf(capture.rdbuf());

  sink.Write({});
  sink.Flush();

  std::cout.rdbuf(old_buf);
  EXPECT_EQ(capture.str(), "");
}

TEST(ConsoleSinkTest, NonNewlineInputGetsTrailingNewline) {
  sink::ConsoleSink sink;

  std::ostringstream capture;
  auto *old_buf = std::cout.rdbuf(capture.rdbuf());

  sink.Write(ToBytes("hello console"));
  sink.Flush();

  std::cout.rdbuf(old_buf);
  EXPECT_EQ(capture.str(), "hello console\n");
}

TEST(ConsoleSinkTest, ExistingNewlineIsNotDuplicated) {
  sink::ConsoleSink sink;

  std::ostringstream capture;
  auto *old_buf = std::cout.rdbuf(capture.rdbuf());

  sink.Write(ToBytes("line\n"));
  sink.Flush();

  std::cout.rdbuf(old_buf);
  EXPECT_EQ(capture.str(), "line\n");
}

TEST(FileSinkTest, EmptyWriteDoesNotEmitContent) {
  const auto path = std::filesystem::temp_directory_path() /
                    "log_daemon_file_sink_empty_test.log";
  std::error_code ec;
  std::filesystem::remove(path, ec);

  {
    sink::FileSink sink(path.string());
    sink.Write({});
    sink.Flush();
  }

  std::ifstream in(path);
  ASSERT_TRUE(in.is_open());

  std::string line;
  EXPECT_FALSE(static_cast<bool>(std::getline(in, line)));

  std::filesystem::remove(path, ec);
}

TEST(FileSinkTest, RelativePathWithoutParentWorks) {
  const std::filesystem::path path = "file_sink_relative_branch_test.log";
  std::error_code ec;
  std::filesystem::remove(path, ec);

  {
    sink::FileSink sink(path.string());
    sink.Write(ToBytes("relative path"));
    sink.Flush();
  }

  std::ifstream in(path);
  ASSERT_TRUE(in.is_open());

  std::string line;
  ASSERT_TRUE(static_cast<bool>(std::getline(in, line)));
  EXPECT_EQ(line, "relative path");

  std::filesystem::remove(path, ec);
}

TEST(FileSinkTest, ExistingNewlineIsNotDuplicatedInFile) {
  const auto path = std::filesystem::temp_directory_path() /
                    "log_daemon_file_sink_newline_test.log";
  std::error_code ec;
  std::filesystem::remove(path, ec);

  {
    sink::FileSink sink(path.string());
    sink.Write(ToBytes("line\n"));
    sink.Flush();
  }

  std::ifstream in(path);
  ASSERT_TRUE(in.is_open());
  std::string line;
  ASSERT_TRUE(static_cast<bool>(std::getline(in, line)));
  EXPECT_EQ(line, "line");
  EXPECT_FALSE(static_cast<bool>(std::getline(in, line)));

  std::filesystem::remove(path, ec);
}

TEST(FileSinkTest, UnwritablePathThrows) {
  EXPECT_THROW(
      {
        sink::FileSink sink("/proc/log_daemon_file_sink_forbidden.log");
      },
      std::runtime_error);
}

TEST(SinkManagerTest, FanOutWritesAndFlushesAllSinks) {
  auto sink_one = std::make_unique<MemorySink>();
  auto sink_two = std::make_unique<MemorySink>();

  MemorySink *raw_one = sink_one.get();
  MemorySink *raw_two = sink_two.get();

  std::vector<std::unique_ptr<sink::ISink>> sinks;
  sinks.push_back(std::move(sink_one));
  sinks.push_back(std::move(sink_two));

  sink::SinkManager manager(std::move(sinks));
  manager.Write(ToBytes("fanout"));
  manager.Flush();

  EXPECT_EQ(raw_one->Data(), "fanout");
  EXPECT_EQ(raw_two->Data(), "fanout");
  EXPECT_TRUE(raw_one->Flushed());
  EXPECT_TRUE(raw_two->Flushed());
}

TEST(SinkManagerTest, EmptySinkListIsNoOp) {
  std::vector<std::unique_ptr<sink::ISink>> sinks;
  sink::SinkManager manager(std::move(sinks));
  manager.Write(ToBytes("ignored"));
  manager.Flush();
}

} // namespace
