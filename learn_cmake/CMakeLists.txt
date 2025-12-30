cmake_minimum_required(VERSION 3.10)
project(learn_cmake)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(test main.cpp include/tcp_srever.hpp)

target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})

