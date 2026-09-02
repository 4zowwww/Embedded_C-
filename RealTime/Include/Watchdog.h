#pragma once
#include <chrono>
#include <mutex>

class Watchdog
{
private:
    using Clock = std::chrono::steady_clock;

    Clock::duration timeout;
    Clock::time_point lastKick;

    mutable std::mutex mtx;

public:
    explicit Watchdog(Clock::duration watchdogTimeout);

    void kick();

    bool expired() const;


};