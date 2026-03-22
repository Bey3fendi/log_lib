cmake_minimum_required(VERSION 3.5.0)
project(log_daemon VERSION 1.0.0 LANGUAGES CXX)

include(CTest)

# Build order: utils -> Sink -> log_event -> Formatter -> Queue -> WorkerThread -> Logger
add_subdirectory(src/utils)
add_subdirectory(src/Sink)

# Add log_event library first (needed by Queue and Logger)
add_library(log_event
    src/Logger/src/log_event.cpp)
target_include_directories(log_event PUBLIC src/Logger/include)

add_subdirectory(src/Queue)
add_subdirectory(src/Logger)

add_library(log_daemon SHARED main.cpp)

target_link_libraries(log_daemon PRIVATE logger)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
