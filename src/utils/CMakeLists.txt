add_library(utils INTERFACE)

target_include_directories(utils INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(utils INTERFACE nlohmann_json::nlohmann_json)
