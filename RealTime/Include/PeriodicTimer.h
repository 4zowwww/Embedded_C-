#pragma once
#include <chrono>
#include <thread>
#include "TimingStats.h"
#include "OverrunPolicy.h"

class PeriodicTimer {

    private:

        using Clock = std::chrono::steady_clock;

        Clock::duration period;
        Clock::time_point scheduledStart;


        OverrunPolicy policy;


    public:
        explicit PeriodicTimer(Clock::duration taskPeriod, OverrunPolicy policy);

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

            bool timingFault =
                missed && policy == OverrunPolicy::Fault;


            if (missed && policy == OverrunPolicy::Resync)
            {
                scheduledStart = finish;
            }
            else if (missed && policy == OverrunPolicy::Fault)
            {
                scheduledStart = finish;
            }
            else
            {
                auto spinStart = deadline - std::chrono::microseconds(200);

                std::this_thread::sleep_until(spinStart);

                while (Clock::now() < deadline)
                {
                    // spin
                }

                scheduledStart += period;
            }


            return {
                jitter,
                execution,
                slack,
                missed,
                timingFault
            };
        }


};

