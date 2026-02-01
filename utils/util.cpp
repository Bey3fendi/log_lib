#include "util.h"

namespace util {
namespace posix {
using Signals = std::initializer_list<int>;

int WaitForSignal(Signals signals) {
  sigset_t mask;
  sigemptyset(&mask);

  for (int sig : signals) {
    sigaddset(&mask, sig);
  }

  int received = 0;
  sigwait(&mask, &received);
  return received;
}

} // namespace posix
} // namespace util