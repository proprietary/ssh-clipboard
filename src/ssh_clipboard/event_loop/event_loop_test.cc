#include "ssh_clipboard/event_loop/event_loop.h"

#include <gtest/gtest.h>

namespace {
TEST(EventLoop, Initialization) {
  auto event_loop = ssh_clipboard::event_loop::create_event_loop(1);
}
}  // namespace
