#include "../include/file_sink.h"

#include <filesystem>
#include <stdexcept>
#include <string>

namespace sink {

FileSink::FileSink(std::string path) : path_(std::move(path)) { OpenIfNeeded(); }

FileSink::~FileSink() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (out_.is_open()) {
    out_.flush();
    out_.close();
  }
}

void FileSink::OpenIfNeeded() {
  if (out_.is_open()) {
    return;
  }

  const std::filesystem::path target(path_);
  if (target.has_parent_path()) {
    std::filesystem::create_directories(target.parent_path());
  }

  out_.open(path_, std::ios::out | std::ios::app);
  if (!out_.is_open()) {
    throw std::runtime_error("FileSink failed to open log file: " + path_);
  }
}

void FileSink::Write(const std::vector<uint8_t> &bytes) {
  if (bytes.empty()) {
    return;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  OpenIfNeeded();

  out_.write(reinterpret_cast<const char *>(bytes.data()),
             static_cast<std::streamsize>(bytes.size()));
  if (bytes.back() != static_cast<uint8_t>('\n')) {
    out_.put('\n');
  }
  out_.flush();
}

void FileSink::Flush() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (out_.is_open()) {
    out_.flush();
  }
}

} // namespace sink
