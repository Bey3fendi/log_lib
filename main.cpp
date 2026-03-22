#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>

constexpr auto app_info = "log_daemon";

int main() {
  auto result = EXIT_FAILURE;

  std::cout << app_info << "start" << std::endl;
}
