cmake_minimum_required(VERSION 3.16)

project(PointerReferencePractice)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app
    src/main.cpp
    src/Sensor.cpp
    src/TelemetryManager.cpp
)

target_include_directories(app PRIVATE include)