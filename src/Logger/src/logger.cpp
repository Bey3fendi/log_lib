#include "../include/logger.h"

#include <memory>
#include <utility>
#include <vector>

namespace logger {

void SLogger::init(std::string config_input) {
  auto [app_name, log_level, pid] = log_config_parser(config_input);
  app_name_ = app_name;
  config_log_level_ = log_level;
  pid_ = pid;

  std::vector<std::unique_ptr<sink::ISink>> sinks;
  auto sink_factory = std::make_unique<sink::SinkFactory>();
  sinks.push_back(sink_factory->Create(sink::SinkFactoryEnum::kConsole));
  sinks.push_back(sink_factory->Create(sink::SinkFactoryEnum::kFile));

  sinkManager_ = std::make_unique<sink::SinkManager>(std::move(sinks));
  formatter_ = std::make_unique<queue::Formatter>();
  queue_ = std::make_unique<queue::BlockingQueue>();

  worker_ = std::make_unique<queue::Worker>(queue_.get(), formatter_.get(),
                                            sinkManager_.get());
}

void SLogger::start() { work_ = std::thread([this]() { worker_->run(); }); }

bool SLogger::shutdown() {
  queue_->stop();

  if (!work_.joinable()) {
    return false;
  }

  work_.join();
  return true;
}

void SLogger::log(std::string_view log_level, std::string_view payload) {
  auto log_event = std::make_unique<LogEvent>(log_level, payload, app_name_);
  queue_->TryPush(std::move(log_event));
}

void SLogger::log(const std::string_view payload) {
  auto log_event = std::make_unique<LogEvent>(config_log_level_, payload, app_name_);
  queue_->TryPush(std::move(log_event));
}

} // namespace logger
