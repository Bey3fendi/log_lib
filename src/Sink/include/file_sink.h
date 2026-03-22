#pragma once

#include "sink_interface.h"

#include <fstream>
#include <mutex>
#include <string>

namespace sink {

class FileSink : public ISink {
public:
  explicit FileSink(std::string path = "/tmp/log_daemon.log");
  ~FileSink() override;

  void Write(const std::vector<uint8_t> &bytes) override;
  void Flush() override;

private:
  void OpenIfNeeded();

  std::string path_;
  std::ofstream out_;
  std::mutex mutex_;
};

} // namespace sink
