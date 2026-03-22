#include "constants.h"
#include "src/Logger/include/log_event.h"
#include "src/Queue/include/blocking_queue.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

namespace {

std::unique_ptr<logger::LogEvent> MakeEvent(const std::string &payload) {
  return std::make_unique<logger::LogEvent>("INFO", payload, "test-app");
}

TEST(BlockingQueueTest, NullEventIsIgnoredAndStopReturnsFalseWhenEmpty) {
  queue::BlockingQueue queue;

  std::unique_ptr<logger::LogEvent> null_event;
  queue.TryPush(std::move(null_event));

  std::unique_ptr<logger::LogEvent> out;
  queue.stop();
  EXPECT_FALSE(queue.PopWait(out));
  EXPECT_EQ(out, nullptr);
}

TEST(BlockingQueueTest, PushAndPopTransfersOwnership) {
  queue::BlockingQueue queue;
  queue.TryPush(MakeEvent("payload-A"));

  std::unique_ptr<logger::LogEvent> out;
  ASSERT_TRUE(queue.PopWait(out));
  ASSERT_NE(out, nullptr);
  EXPECT_EQ(out->GetPayload(), "payload-A");
}

TEST(BlockingQueueTest, OverflowDropsOldestMessage) {
  queue::BlockingQueue queue;

  for (int i = 0; i < QUEUE_SIZE + 2; ++i) {
    queue.TryPush(MakeEvent(std::to_string(i)));
  }

  queue.stop();

  std::vector<std::string> payloads;
  std::unique_ptr<logger::LogEvent> out;
  while (queue.PopWait(out)) {
    payloads.push_back(out->GetPayload());
  }

  ASSERT_EQ(payloads.size(), static_cast<size_t>(QUEUE_SIZE + 1));
  EXPECT_EQ(payloads.front(), "1");
  EXPECT_EQ(payloads.back(), std::to_string(QUEUE_SIZE + 1));
}

} // namespace
