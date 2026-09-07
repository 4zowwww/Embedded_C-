#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstddef>
#include <optional>

template<typename T>
class BoundedQueue
{
private:
    std::queue<T> queue;

    std::size_t capacity;

    std::mutex mtx;

    std::condition_variable notEmpty;
    std::condition_variable notFull;

    bool closed = false;

public:
    explicit BoundedQueue(std::size_t maxCapacity)
        : capacity(maxCapacity)
    {
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> lock(mtx);

        notFull.wait(lock, [&]()
        {
            return queue.size() < capacity;
        });

        queue.push(item);

        lock.unlock();

        notEmpty.notify_one();
    }

    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> lock(mtx);

        notEmpty.wait(lock, [&]()
        {
            return !queue.empty() || closed;
        });

        if (queue.empty() && closed)
        {
            return std::nullopt;
        }

        T item = std::move(queue.front());
        queue.pop();

        lock.unlock();

        notFull.notify_one();

        return item;
    }


    bool tryPush(const T& item)
    {
        std::unique_lock<std::mutex> lock(mtx);

        if (queue.size() >= capacity)
        {
            return false;
        }

        queue.push(item);

        lock.unlock();

        notEmpty.notify_one();

        return true;
    }

    void close()
    {
        std::lock_guard<std::mutex> lock(mtx);

        closed = true;

        notEmpty.notify_all();
        notFull.notify_all();
    }
};