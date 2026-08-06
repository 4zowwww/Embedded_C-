#pragma once
#include <algorithm>
#include <numeric>
#include <array>
#include <cstddef>
#include <optional>

template <typename T, std::size_t Capacity>
class SensorBuffer
{
    static_assert(Capacity > 0,
        "SensorBuffer capacity must be greater than zero.");
public:
    bool add(const T& value)
    {
        data_[writeIndex_] = value;

        writeIndex_ = (writeIndex_ + 1) % Capacity;

        if (count_ < Capacity)
        {
            ++count_;
        }

        return true;
    }

    std::size_t size() const
    {
        return count_;
    }


    constexpr std::size_t capacity() const
    {
        return Capacity;
    }


    bool full() const
    {
        return count_ >= Capacity;
    }

    bool empty() const
    {
        return count_ == 0;
    }


    void clear()
    {
        count_ = 0;
        writeIndex_ = 0;
    }

    std::optional<T> get(std::size_t index) const
    {
        if (index >= count_)
            return std::nullopt;

        std::size_t oldestIndex = 0;

        if (full())
        {
            oldestIndex = writeIndex_;
        }

        const std::size_t physicalIndex =
            (oldestIndex + index) % Capacity;

        return data_[physicalIndex];
    }

    std::optional<T> latest() const
    {
        if (empty())
            return std::nullopt;

        const std::size_t latestIndex =
            (writeIndex_ + Capacity - 1) % Capacity;

        return data_[latestIndex];
    }

    std::optional<double> average() const
    {
        if (empty())
            return std::nullopt;

        const double sum = std::accumulate(
            data_.begin(),
            data_.begin() + count_,
            0.0
        );

        return sum / static_cast<double>(count_);
    }

    std::optional<T> oldest() const
    {
        if (empty())
            return std::nullopt;

        if (full())
            return data_[writeIndex_];

        return data_[0];
    }


    std::optional<T> minimum() const
    {
        if (empty())
            return std::nullopt;

        const auto smallestIterator = std::min_element(
            data_.begin(),
            data_.begin() + count_
        );

        return *smallestIterator;
    }

    std::optional<T> maximum() const
    {
        if (empty())
            return std::nullopt;

        const auto largestIterator = std::max_element(
            data_.begin(),
            data_.begin() + count_
        );

        return *largestIterator;
    }

    bool contains(const T& value) const
    {
        const auto endOfValidData =
            data_.begin() + count_;

        const auto found = std::find(
            data_.begin(),
            endOfValidData,
            value
        );

        return found != endOfValidData;
    }

    std::size_t countAbove(const T& threshold) const
    {
        return static_cast<std::size_t>(
            std::count_if(
                data_.begin(),
                data_.begin() + count_,
                [&threshold](const T& reading)
                {
                    return reading > threshold;
                }
            )
        );
    }

private:
    std::array<T, Capacity> data_{};
    std::size_t count_{0};
    std::size_t writeIndex_{0};
};