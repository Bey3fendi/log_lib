#include "../include/sink_manager.h"

namespace sink {

void SinkManager::Write(const std::vector<uint8_t> &bytes) {
  for (auto &sink : sinks_) {
    sink->Write(bytes);
  }
}

void SinkManager::Flush() {
  for (auto &sink : sinks_) {
    sink->Flush();
  }
}

} // namespace sink
