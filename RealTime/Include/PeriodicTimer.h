#pragma once
#include <chrono>
#include <thread>

struct TimingStats
{
    std::chrono::microseconds jitter;
    std::chrono::microseconds execution;
    std::chrono::microseconds slack;

    bool deadlineMissed;
};

class PeriodicTimer {

    private:

        using Clock = std::chrono::steady_clock;

        Clock::duration period;
        Clock::time_point scheduledStart;



    public:
        explicit PeriodicTimer(Clock::duration taskPeriod);

        template<typename Task>
        TimingStats runCycle(Task&& task)
        {
            auto actualStart = Clock::now();

            auto jitter =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    actualStart - scheduledStart
                );


            auto deadline =
                scheduledStart + period;


            task();


            auto finish = Clock::now();


            auto execution =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    finish - actualStart
                );


            auto slack =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    deadline - finish
                );


            bool missed =
                finish > deadline;


            std::this_thread::sleep_until(deadline);

            scheduledStart += period;


            return {
                jitter,
                execution,
                slack,
                missed
            };
        }

};

