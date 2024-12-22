cmake_minimum_required(VERSION 3.18)

add_executable(ut
    main.cc
    data_stream_test.cc
)

target_include_directories(ut PRIVATE ../src)
target_link_libraries(ut PRIVATE cpp-serialize)
target_link_libraries(ut PRIVATE gtest)
