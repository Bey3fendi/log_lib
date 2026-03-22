find_package(GTest QUIET)

if(NOT GTest_FOUND)
  message(WARNING "GTest not found. Install libgtest-dev (or use updated Docker image) to build unit_test.")
  return()
endif()

add_executable(unit_test
  formatter_test.cpp
  logger_test.cpp
  parser_test.cpp
  queue_test.cpp
  sink_factory_test.cpp
  log_event_test.cpp
  sink_test.cpp
)

target_include_directories(unit_test
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(unit_test
  PRIVATE
    logger
    worker_thread
    queue
    formatter
    sink
    sink_manager
    sink_interface
    log_event
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME unit_test COMMAND unit_test)
