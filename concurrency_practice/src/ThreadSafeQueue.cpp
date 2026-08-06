#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include "ThreadSafeQueue.hpp"




std::queue<int> values;
std::mutex mutex;
std::condition_variable condition;

void ThreadSafeQueue::push(int value)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);

            values.push(value);
        }

        condition.notify_one();
    }

int ThreadSafeQueue::waitAndPop()
    {   
        int copy_values;

        {
        std::unique_lock<std::mutex> lock(mutex);

        condition.wait(
            lock,
            [this]
            {
                return !values.empty();
            });
        }
        
        copy_values = values.front();
        values.pop();
        
        return copy_values;
    }    
