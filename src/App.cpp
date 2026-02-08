#include "App.h"
#include <ios>
#include <iostream>

namespace log_daemon {
bool App::Init() {
  bool ret = false;
  std::cout << "App initialized start" << std::endl;

  //...
  ret = true; //temporary
  if (ret) {
    std::cout << "App initialize gracefully flag returned as: "
              << std::boolalpha << ret << std::endl;
  } else {
    std::cout << "App initialize failed flag returned as: " << std::boolalpha
              << ret << std::endl;
  }

  return ret;
}

bool App::Start() {
  bool ret = false;
  std::cout << "App start" << std::endl;

  //...
  ret = true; //temporary
  if (ret) {
    std::cout << "App start gracefully flag returned as: " << std::boolalpha
              << ret << std::endl;
  } else {
    std::cout << "App start failed flag returned as: " << std::boolalpha << ret
              << std::endl;
  }

  return ret;
}

bool App::Stop() {
  bool ret = false;
  std::cout << "App stop" << std::endl;

  //...
  ret = true; //temporary
  if (ret) {
    std::cout << "App stop gracefully flag returned as: " << std::boolalpha
              << ret << std::endl;
  } else {
    std::cout << "App stop failed flag returned as: " << std::boolalpha << ret
              << std::endl;
  }

  return ret;
}
} // namespace log_daemon