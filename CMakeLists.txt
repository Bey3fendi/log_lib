cmake_minimum_required(VERSION 3.5.0)

project(log_daemon VERSION 1.0.0)

add_subdirectory(src)
add_subdirectory(utils)


add_executable(log_daemon main.cpp)

target_link_libraries(log_daemon PUBLIC app util)