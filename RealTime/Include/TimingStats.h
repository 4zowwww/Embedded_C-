#pragma once
#include <chrono>

struct TimingStats
{
    std::chrono::microseconds jitter;
    std::chrono::microseconds execution;
    std::chrono::microseconds slack;

    bool deadlineMissed;
    bool timingFault;
};
