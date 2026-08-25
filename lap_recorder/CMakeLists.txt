cmake_minimum_required(VERSION 3.10)

project(lap_recorder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(app 
    src/main.cpp
    src/StintLog.cpp
    src/LapTelemetryCodec.cpp
    tests/StintLogTests.cpp
    tests/LapTelemetryCodecTests.cpp
        src/ThreadSafeLapQueue.cpp

)

target_include_directories(app PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)