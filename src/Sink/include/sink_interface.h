#pragma once

#include <cstdint>
#include <vector>

namespace sink {

class ISink {
public:
  virtual ~ISink() = default;
  virtual void Write(const std::vector<uint8_t> &bytes) = 0;
  virtual void Flush() {}
};

} // namespace sink
