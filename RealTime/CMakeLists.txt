cmake_minimum_required(VERSION 3.20)

project(RealTime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
        main.cpp
        src/PeriodicTimer.cpp
        src/TimingMonitor.cpp
        src/Watchdog.cpp
)

target_include_directories(app PRIVATE include)