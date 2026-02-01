#include "App.h"
#include <ios>
#include <iostream>

namespace log_daemon {
bool App::Init() {
  bool ret = false;
  std::cout << "App initialized start" << "\n";

  //...
  ret = true; //temporary
  if (ret) {
    std::cout << "App initialize gracefully flag returned as: "
              << std::boolalpha << ret << "\n";
  } else {
    std::cout << "App initialize failed flag returned as: " << std::boolalpha
              << ret << "\n";
  }

  return ret;
}

bool App::Start() {
  bool ret = false;
  std::cout << "App start" << "\n";

  //...
  ret = true; //temporary
  if (ret) {
    std::cout << "App start gracefully flag returned as: " << std::boolalpha
              << ret << "\n";
  } else {
    std::cout << "App start failed flag returned as: " << std::boolalpha << ret
              << "\n";
  }

  return ret;
}

bool App::Stop() {
  bool ret = false;
  std::cout << "App stop" << "\n";

  //...
  ret = true; //temporary
  if (ret) {
    std::cout << "App stop gracefully flag returned as: " << std::boolalpha
              << ret << "\n";
  } else {
    std::cout << "App stop failed flag returned as: " << std::boolalpha << ret
              << "\n";
  }

  return ret;
}
} // namespace log_daemon