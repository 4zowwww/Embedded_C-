cmake_minimum_required(VERSION 3.20)

project(src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(threadpool
        src/ThreadPool.cpp
        src/main.cpp
)

target_include_directories(threadpool PRIVATE include)