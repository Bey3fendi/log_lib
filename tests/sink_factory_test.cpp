#include "src/Sink/include/console_sink.h"
#include "src/Sink/include/file_sink.h"
#include "src/Sink/include/sink_factory.h"

#include <gtest/gtest.h>

TEST(SinkFactoryTest, CreatesConsoleSinkForConsoleEnum) {
  sink::SinkFactory factory;
  auto created = factory.Create(sink::SinkFactoryEnum::kConsole);
  EXPECT_NE(dynamic_cast<sink::ConsoleSink *>(created.get()), nullptr);
}

TEST(SinkFactoryTest, CreatesFileSinkForFileEnum) {
  sink::SinkFactory factory;
  auto created = factory.Create(sink::SinkFactoryEnum::kFile);
  EXPECT_NE(dynamic_cast<sink::FileSink *>(created.get()), nullptr);
}
