#include "ThreadPool.h"

#include <cstddef>
#include <emmintrin.h>
#include <functional>
#include <mutex>
#include <thread>

ThreadPool::ThreadPool(std::size_t threadCount) {

    for ( std::size_t i = 0; i < threadCount; ++i ) {

        threads.emplace_back([this] {

            while (true) {

                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(mutex);

                    condition.wait(lock, [this] {

                        return !tasks.empty() || stop;
                    });

                    if (stop && tasks.empty())
                    {
                        return;
                    }

                    task = std::move(tasks.front());

                    tasks.pop();

                }

                task();
            }
        });
    }
}



ThreadPool::~ThreadPool() {

    {
        std::lock_guard<std::mutex> lock(mutex);

        stop = true;
    }

    condition.notify_all();

    for (std::thread &thread : threads) {

        if (thread.joinable()) {
            thread.join();
        }
    }
}
