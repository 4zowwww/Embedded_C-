#pragma once

#include "PeriodicTimer.h"
#include "TimingStats.h"

class TimingMonitor
{
private:
    long long totalExecution = 0;
    long long minExecution = 0;
    long long maxExecution = 0;

    int samples = 0;
    int deadlineMisses = 0;

public:
    void update(const TimingStats& stats);

    double averageExecution() const;

    void print() const;
};