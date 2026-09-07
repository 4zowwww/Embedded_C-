#pragma once

#include <array>
#include <cstddef>

template<typename T, std::size_t Capacity>
class RingBuffer
{
private:

    std::array<T, Capacity> buffer;

    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t size = 0;

public:

    void push(const T& item)
    {
        buffer[head] = item;

        head = (head + 1) % Capacity;

        if (size < Capacity)
        {
            size++;
        }
        else
        {
            tail = (tail + 1) % Capacity;
        }
    }

    std::optional<T> pop()
    {
        if (size == 0)
        {
            return std::nullopt;
        }

        T item = std::move(buffer[tail]);

        tail = (tail + 1) % Capacity;
        size--;

        return item;
    }

    bool empty() const
    {
        return size == 0;
    }

    bool full() const
    {
        return size == Capacity;
    }

    std::size_t currentSize() const
    {
        return size;
    }

};