add_library(sink_interface 
    src/sink_interface.cpp)

target_include_directories(sink_interface PUBLIC include)


add_library(sink 
    src/console_sink.cpp
    src/file_sink.cpp)

target_include_directories(sink PUBLIC include)

target_link_libraries(sink PUBLIC sink_interface)


add_library(sink_manager 
    src/sink_manager.cpp)

target_include_directories(sink_manager PUBLIC include)

target_link_libraries(sink_manager PUBLIC sink sink_interface)
