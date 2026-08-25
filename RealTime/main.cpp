#include <iostream>
#include <chrono>
#include <thread>

#include "PeriodicTimer.h"


void controlTask()
{
    std::this_thread::sleep_for(
        std::chrono::milliseconds(3)
    );
}


int main()
{
    using namespace std::chrono_literals;

    PeriodicTimer timer(10ms);


    for (int cycle = 0; cycle < 5; ++cycle)
    {
        TimingStats stats =
            timer.runCycle(controlTask);


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
}