#include <csignal>
#include <initializer_list>

namespace util {
namespace posix {
using Signals = std::initializer_list<int>;

constexpr Signals signals{SIGILL, SIGABRT, SIGBUS, SIGFPE, SIGSEGV};

int WaitForSignal(Signals signals);
} // namespace posix
} // namespace util