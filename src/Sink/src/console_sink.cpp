#include "../include/console_sink.h"

#include <iostream>
#include <string>

namespace sink {

void ConsoleSink::Write(const std::vector<uint8_t> &bytes) {
  if (bytes.empty()) {
    return;
  }

  const std::string line(bytes.begin(), bytes.end());
  std::cout << line;
  if (line.back() != '\n') {
    std::cout << '\n';
  }
}

void ConsoleSink::Flush() { std::cout << std::flush; }

} // namespace sink
