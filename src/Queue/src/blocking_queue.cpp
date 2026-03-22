#include "../include/blocking_queue.h"

namespace queue {

void BlockingQueue::TryPush(std::unique_ptr<logger::LogEvent> log_event) {
  if (!log_event) {
    return;
  }

  {
    std::lock_guard<std::mutex> lock(mtx_);
    if (b_queue_.size() > QUEUE_SIZE) {
      b_queue_.pop();
      ++message_drop_count_;
    }
    b_queue_.push(std::move(log_event));
  }

  block_cv_.notify_one();
}

bool BlockingQueue::PopWait(std::unique_ptr<logger::LogEvent> &out) {
  std::unique_lock<std::mutex> lock(mtx_);
  block_cv_.wait(lock, [&] { return stopping_ || !b_queue_.empty(); });

  if (stopping_ && b_queue_.empty()) {
    return false;
  }

  out = std::move(b_queue_.front());
  b_queue_.pop();
  return true;
}

void BlockingQueue::stop() {
  {
    std::lock_guard<std::mutex> lock(mtx_);
    stopping_ = true;
  }
  block_cv_.notify_all();
}

} // namespace queue
