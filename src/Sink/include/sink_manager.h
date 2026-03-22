#pragma once

#include "sink_interface.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace sink {

class SinkManager {
public:
  explicit SinkManager(std::vector<std::unique_ptr<sink::ISink>> sinks)
      : sinks_(std::move(sinks)) {}

  void Write(const std::vector<uint8_t> &bytes);
  void Flush();

private:
  std::vector<std::unique_ptr<sink::ISink>> sinks_;
};

} // namespace sink
