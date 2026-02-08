#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdlib.h>

#include "src/App.h"

constexpr auto app_info = "log_daemon";

int main() {
  auto result = EXIT_FAILURE;

  std::cout << app_info << "start" << std::endl;

  try {
    log_daemon::App app{};
    if (!app.Init()) {
      std::cout << "App init failed!" << std::endl;
    } else if (!app.Start()) {
      std::cout << "App start failed!" << std::endl;
    } else {
        // auto signal = ::util::posix::WaitForSignal({SIGINT, SIGTERM});

        if(app.Stop()) {
            result = EXIT_SUCCESS;
        }

        std::cout << "App stopped cause of signal: " << signal << std::endl;
    }
  } catch (const std::exception &e) {
    std::cout << "exception thrown: " << e.what() << std::endl;
  } catch (...) {
    std::cout << "unknown exception thrown" << std::endl;
  }
}
