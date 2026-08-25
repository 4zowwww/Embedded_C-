#include "ThreadSafeLapQueue.hpp"
#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <iostream>
#include <queue>
#include <mutex>

#include "LapRecord.hpp"

void ThreadSafeLapQueue::push(const LapRecord& lap) {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        lapRecordQueue_.push(lap);
    }

    condition_.notify_one();
}



lapRecord ThreadSafeLapQueue::waitAndPop() {

        std::unique_lock<std::mutex> lock(mutex_);

        condition_.wait(
            lock,
            [this] {
                return !lapRecordQueue_.empty();
            }
            );

        LapRecord copy = lapRecordQueue_.front();

        lapRecordQueue_.pop();

        return copy;
}

