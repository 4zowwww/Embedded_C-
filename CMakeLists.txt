cmake_minimum_required(VERSION 3.16)

project(TemplatesPractice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
    src/main.cpp
        lap_recorder/include/ThreadSafeLapQueue.hpp
        lap_recorder/src/ThreadSafeLapQueue.cpp
        ThreadPool/include/ThreadPool.h
        ThreadPool/src/ThreadPool.cpp
        ThreadPool/include/ThreadPool.h
        ThreadPool/src/ThreadPool.cpp
        ThreadPool_Final/main.cpp
        ThreadPool_Final/include/ThreadPool.h
        ThreadPool_Final/src/ThreadPool.cpp
        RealTime/main.cpp
        RealTime/src/PeriodicTimer.cpp
        RealTime/Include/PeriodicTimer.h
        RealTime/Include/TimingMonitor.h
        RealTime/src/TimingMonitor.cpp
        RealTime/Include/TimingStats.h
        RealTime/Include/OverrunPolicy.h
        RealTime/Include/Watchdog.h
        RealTime/src/Watchdog.cpp
        RealTime/src/Watchdog.cpp
)

target_include_directories(app PRIVATE include)
