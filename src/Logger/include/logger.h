#pragma once

#include "../../Queue/include/blocking_queue.h"
#include "../../Queue/include/formatter.h"
#include "../../Queue/include/worker_thread.h"
#include "../../Sink/include/sink_factory.h"
#include "../../Sink/include/sink_manager.h"
#include "../../utils/include/json_parser.h"

#include <iostream>
#include <memory>
#include <thread>

namespace logger {

class SLogger {
public:
  SLogger(const SLogger &) = delete;
  SLogger &operator=(const SLogger &) = delete;

  static SLogger &getInstance() {
    static SLogger instance;
    return instance;
  }

  void init(std::string config_input);
  void start();
  bool shutdown();
  void log(std::string &log_level, std::string &payload);

private:
  std::string app_name_{};
  std::string config_log_level_{};
  std::string pid_{};

  std::unique_ptr<queue::BlockingQueue> queue_;
  std::unique_ptr<queue::Formatter> formatter_;
  std::unique_ptr<sink::SinkManager> sinkManager_;
  std::unique_ptr<queue::Worker> worker_;

  std::thread work_;

  SLogger() = default;
  ~SLogger() = default;
};

} // namespace logger
