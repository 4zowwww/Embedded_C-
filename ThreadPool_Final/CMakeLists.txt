cmake_minimum_required(VERSION 3.16)

project(ThreadPoolFinal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(threadpool_final
        main.cpp
        src/ThreadPool.cpp
)

target_include_directories(threadpool_final PRIVATE
        include
)