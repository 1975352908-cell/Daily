cmake_minimum_required(VERSION 3.10)
project(learn_socket)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(test test.cpp include/tcp.hpp)

target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})