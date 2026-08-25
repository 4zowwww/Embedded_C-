#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;

public:
    explicit ThreadPool(std::size_t threadCount);

    ~ThreadPool();

    template<typename F>
    auto submit(F&& function)
        -> std::future<std::invoke_result_t<F>>
    {
        using ReturnType = std::invoke_result_t<F>;

        auto taskPtr =
            std::make_shared<std::packaged_task<ReturnType()>>(
                std::forward<F>(function)
            );

        std::future<ReturnType> result =
            taskPtr->get_future();

        {
            std::lock_guard<std::mutex> lock(queueMutex);

            if (stop)
            {
                throw std::runtime_error("src is stopped");
            }

            tasks.push([taskPtr]
            {
                (*taskPtr)();
            });
        }

        condition.notify_one();

        return result;
    }
};