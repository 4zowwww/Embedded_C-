#pragma once
#include <optional>


template <typename T>
class SensorValue
{
public:
    SensorValue(T minimum, T maximum)
        : minimum_(minimum),
          maximum_(maximum)
    {
    }


    const std::optional<T>& get() const
    {
        return value_;
    }

    bool set(T value)
    {
        if (value < minimum_ || value > maximum_)
        {
            return false;
        }

        value_ = value;
        return true;
    }

    bool hasValue() const
    {
        return value_.has_value();
    }

    void clear()
    {
        value_.reset();
    }

private:
    std::optional<T> value_;
    T minimum_;
    T maximum_;
};