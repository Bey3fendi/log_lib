add_library(formatter 
    src/formatter.cpp)

target_include_directories(formatter PUBLIC include)

target_link_libraries(formatter PUBLIC sink_manager)


add_library(queue 
    src/blocking_queue.cpp)

target_include_directories(queue PUBLIC include)

target_link_libraries(queue PUBLIC formatter log_event)


add_library(worker_thread
    src/worker_thread.cpp)

target_include_directories(worker_thread PUBLIC include)

target_link_libraries(worker_thread PUBLIC formatter queue sink_manager)
