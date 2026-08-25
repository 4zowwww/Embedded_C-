#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <cstddef>

class ThreadSafeQueue
{
public:
    void producer(int id);
    void consumer(int id);
    void close();
    void worker();

private:
    std::queue<int> pendingValues_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::condition_variable spaceAvailable_;
    bool closed_ = false;
    static constexpr std::size_t capacity_ = 5;
};