cmake_minimum_required (VERSION 2.8)
project (basic)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

add_executable(test test.cpp)

find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(test PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(test "${CMAKE_THREAD_LIBS_INIT}")
endif()
