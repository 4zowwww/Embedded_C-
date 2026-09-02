#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include "PeriodicTimer.h"
#include "TimingMonitor.h"
#include "OverrunPolicy.h"
#include "Watchdog.h"
#include "TimingStats.h"


void controlTask()
{
    std::this_thread::sleep_for(
        std::chrono::milliseconds(250)
    );
}


int main()
{
    using namespace std::chrono_literals;

    PeriodicTimer timer(10ms, OverrunPolicy::Resync);
    TimingMonitor monitor;
    Watchdog watchdog(100ms);
    std::atomic<bool> stopWatchdog{false};

    std::thread watchdogThread([&]()
    {
        while (!stopWatchdog)
        {
            if (watchdog.expired())
            {
                std::cout << "WATCHDOG EXPIRED\n";
            }

            std::this_thread::sleep_for(10ms);
        }
    });

    for (int cycle = 0; cycle < 5; ++cycle)
    {
        TimingStats stats =
            timer.runCycle(controlTask);

        if (stats.timingFault)
        {
            // higher-level system decides what happens
        }

        if (stats.deadlineMissed)
        {
            watchdog.kick();
        }


        monitor.update(stats);

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
    watchdogThread.join();

    monitor.print();
}