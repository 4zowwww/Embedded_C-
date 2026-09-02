#include "TimingMonitor.h"

#include <iostream>


void TimingMonitor::update(const TimingStats& stats)
{
    long long execution =
        stats.execution.count();


    if (samples == 0)
    {
        minExecution = execution;
        maxExecution = execution;
    }
    else
    {
        if (execution < minExecution)
        {
            minExecution = execution;
        }

        if (execution > maxExecution)
        {
            maxExecution = execution;
        }
    }


    totalExecution += execution;


    if (stats.deadlineMissed)
    {
        deadlineMisses++;
    }


    samples++;
}

double TimingMonitor::averageExecution() const {
    return totalExecution / samples;
}

void TimingMonitor::print() const
{
    std::cout << "\n--- Timing Summary ---\n";

    std::cout
        << "Samples: "
        << samples
        << '\n';

    std::cout
        << "Min execution: "
        << minExecution
        << " us\n";

    std::cout
        << "Average execution: "
        << averageExecution()
        << " us\n";

    std::cout
        << "Max execution: "
        << maxExecution
        << " us\n";

    std::cout
        << "Deadline misses: "
        << deadlineMisses
        << '\n';
}