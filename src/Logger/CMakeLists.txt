add_library(logger 
    src/logger.cpp)

target_include_directories(logger PUBLIC include)

target_link_libraries(logger PUBLIC utils queue worker_thread log_event)
