#pragma once

#include "../../Queue/include/blocking_queue.h"
#include "../../Queue/include/formatter.h"
#include "../../Sink/include/sink_manager.h"

#include <cstdint>
#include <vector>

namespace queue {

class Worker {
public:
  Worker(queue::BlockingQueue *queue, queue::Formatter *formatter,
         sink::SinkManager *sink_manager)
      : queue_(queue), formatter_(formatter), sink_manager_(sink_manager) {}

  ~Worker() = default;

  void run() {
    std::unique_ptr<logger::LogEvent> event;
    while (queue_->PopWait(event)) {
      std::vector<uint8_t> buf;
      formatter_->LogFormatter(event, buf);
      sink_manager_->Write(buf);
    }
    sink_manager_->Flush();
  }

private:
  queue::BlockingQueue *queue_;
  queue::Formatter *formatter_;
  sink::SinkManager *sink_manager_;
};

} // namespace queue
