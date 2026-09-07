#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "PeriodicTimer.h"
#include "TimingMonitor.h"
#include "OverrunPolicy.h"
#include "Watchdog.h"
#include "TimingStats.h"
#include "BoundedQueue.h"
#include "RingBuffer.h"

struct ControlStepTimings
{
    std::chrono::microseconds sensor;
    std::chrono::microseconds filter;
    std::chrono::microseconds control;
    std::chrono::microseconds can;
};


void controlTask(ControlStepTimings& timings)
{
    using Clock = std::chrono::steady_clock;
    using namespace std::chrono_literals;

    auto start = Clock::now();

    // Simulate sensor read
    std::this_thread::sleep_for(1ms);

    auto afterSensor = Clock::now();

    // Simulate filtering
    std::this_thread::sleep_for(1ms);

    auto afterFilter = Clock::now();

    // Simulate control calculation
    std::this_thread::sleep_for(2ms);

    auto afterControl = Clock::now();

    // Simulate CAN transmission
    std::this_thread::sleep_for(1ms);

    auto afterCan = Clock::now();

    timings.sensor =
        std::chrono::duration_cast<std::chrono::microseconds>(
            afterSensor - start);

    timings.filter =
        std::chrono::duration_cast<std::chrono::microseconds>(
            afterFilter - afterSensor);

    timings.control =
        std::chrono::duration_cast<std::chrono::microseconds>(
            afterControl - afterFilter);

    timings.can =
        std::chrono::duration_cast<std::chrono::microseconds>(
            afterCan - afterControl);
}


int main()
{
    using namespace std::chrono_literals;

    PeriodicTimer timer(10ms, OverrunPolicy::Resync);
    TimingMonitor monitor;
    Watchdog watchdog(100ms);
    std::atomic<bool> stopWatchdog{false};
    BoundedQueue<TimingStats> statsQueue(2);
    RingBuffer<TimingStats, 3> recentStats;



    std::jthread monitorThread([&]()
    {
        while (true)
        {
            auto stats = statsQueue.pop();

            if (!stats.has_value())
            {
                break;
            }

            std::this_thread::sleep_for(50ms);

            monitor.update(*stats);
        }
    });

    std::jthread watchdogThread([&]()
    {
        bool alreadyReported = false;

        while (!stopWatchdog)
        {
            if (watchdog.expired())
            {
                if (!alreadyReported)
                {
                    std::cout << "WATCHDOG EXPIRED\n";
                    alreadyReported = true;
                }
            }
            else
            {
                alreadyReported = false;
            }

            std::this_thread::sleep_for(10ms);
        }
    });



    for (int cycle = 0; cycle < 5; ++cycle)
    {
        ControlStepTimings stepTimings;

        TimingStats stats = timer.runCycle([&]()
        {
            controlTask(stepTimings);
        });

        const auto sensorBudget  = 1500us;
        const auto filterBudget  = 1500us;
        const auto controlBudget = 3000us;
        const auto canBudget     = 1000us;

        if (stepTimings.sensor > sensorBudget)
        {
            std::cout << "Sensor: OVER BUDGET\n";
        }
        else
        {
            std::cout << "Sensor: OK\n";
        }

        if (stepTimings.filter > filterBudget)
        {
            std::cout << "Filter: OVER BUDGET\n";
        }
        else
        {
            std::cout << "Filter: OK\n";
        }

        if (stepTimings.control > controlBudget)
        {
            std::cout << "Control: OVER BUDGET\n";
        }
        else
        {
            std::cout << "Control: OK\n";
        }

        if (stepTimings.can > canBudget)
        {
            std::cout << "CAN: OVER BUDGET\n";
        }
        else
        {
            std::cout << "CAN: OK\n";
        }

        if (stats.timingFault)
        {
            // higher-level system decides what happens
        }

        watchdog.kick();

        recentStats.push(stats);
        
        if (!statsQueue.tryPush(stats))
        {
            std::cout << "Telemetry dropped\n";
        }


        std::cout
            << "Cycle " << cycle
            << "\nJitter: "
            << stats.jitter.count()
            << " us"
            << "\nExecution: "
            << stats.execution.count()
            << " us"
            << "\nSlack: "
            << stats.slack.count()
            << " us"
            << "\nStatus: "
            << (stats.deadlineMissed ? "MISS" : "OK")
            << "\n\n";
    }

    
    stopWatchdog = true;

    statsQueue.close();

    monitorThread.join();

    monitor.print();

    std::cout << "\n--- Recent Ring Buffer Stats ---\n";
    while (!recentStats.empty())
    {
        auto stats = recentStats.pop();

        if (stats)
        {
            std::cout
                << "Execution: "
                << (*stats).execution.count()
                << " us\n";
        }
    }
}