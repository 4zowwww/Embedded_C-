#include "ThreadSafeQueue.hpp"

#include <iostream>
#include <chrono>
#include <thread>

void ThreadSafeQueue::producer(int id)
{
    for (int i = 1; i<= 5; ++i)
    {
        int value = id * 100 + i;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            spaceAvailable_.wait(
                lock,
                [this]
                {
                    return pendingValues_.size() < capacity_;
                }
            );
            pendingValues_.push(value);
        }

        condition_.notify_one();
    }
}

void ThreadSafeQueue::consumer(int id)
{
    while (true)
    {
        int value;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            condition_.wait(
                lock,
                [this]
                {
                    return closed_ || !pendingValues_.empty();
                }
            );

            if (closed_ && pendingValues_.empty())
            {
                break;
            }

            value = pendingValues_.front();
            pendingValues_.pop();
        }

        spaceAvailable_.notify_one();

        std::this_thread::sleep_for(
        std::chrono::milliseconds(500)
        );

        std::cout << "Consumer " << id
          << " consumed: " << value << '\n';
    }
}

void ThreadSafeQueue::close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);

        closed_ = true;
    }

    condition_.notify_all();
}

