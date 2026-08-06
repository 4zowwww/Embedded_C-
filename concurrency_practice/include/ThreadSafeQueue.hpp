#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#pragma once



class ThreadSafeQueue
{
private:
    std::queue<int> values;
    std::mutex mutex;
    std::condition_variable condition;

public:
    void push(int value);

    int waitAndPop();

};