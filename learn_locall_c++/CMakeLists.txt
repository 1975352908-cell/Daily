cmake_minimum_required(VERSION 3.10)
project(learn_locall_c++)

set(CMAKE_CXX_STANDARD 11)

add_executable(test thread_test.cpp)

target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})