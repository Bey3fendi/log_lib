#pragma once

#include "sink_interface.h"

namespace sink {

class ConsoleSink : public ISink {
public:
  void Write(const std::vector<uint8_t> &bytes) override;
  void Flush() override;
};

} // namespace sink
