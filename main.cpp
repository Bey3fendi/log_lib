#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdlib.h>

#include "src/App.h"
#include "utils/util.h"

constexpr auto app_info = "log_daemon";

int main() {
  auto result = EXIT_FAILURE;

  std::cout << app_info << "start" << "\n";

  try {
    log_daemon::App app{};
    if (!app.Init()) {
      std::cout << "App init failed!" << "\n";
    } else if (!app.Start()) {
      std::cout << "App start failed!" << "\n";
    } else {
        auto signal = ::util::posix::WaitForSignal({SIGINT, SIGTERM});

        if(app.Stop()) {
            result = EXIT_SUCCESS;
        }

        std::cout << "App stopped cause of signal: " << signal << "\n";
    }
  } catch (const std::exception &e) {
    std::cout << "exception thrown: " << e.what() << "\n";
  } catch (...) {
    std::cout << "unknown exception thrown" << "\n";
  }
}