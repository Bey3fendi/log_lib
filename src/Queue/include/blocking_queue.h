#pragma once

#include "../../Logger/include/log_event.h"
#include "../../../constants.h"

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>

namespace queue {

class BlockingQueue {
public:
  BlockingQueue() = default;
  ~BlockingQueue() = default;

  void TryPush(std::unique_ptr<logger::LogEvent> log_event);
  bool PopWait(std::unique_ptr<logger::LogEvent> &out);
  void stop();

private:
  std::mutex mtx_;
  std::condition_variable block_cv_;

  std::queue<std::unique_ptr<logger::LogEvent>> b_queue_;

  uint16_t message_drop_count_{0};
  bool stopping_{false};
};

} // namespace queue
