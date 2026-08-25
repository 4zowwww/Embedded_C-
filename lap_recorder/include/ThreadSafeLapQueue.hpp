#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

#include "LapRecord.hpp"


class ThreadSafeLapQueue
{
public:
    void push(const LapRecord& lap);
    LapRecord waitAndPop();

private:
    std::queue<LapRecord> lapRecordQueue_;
    std::mutex mutex_;
    std::condition_variable condition_;
};
