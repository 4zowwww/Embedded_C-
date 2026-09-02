#include "PeriodicTimer.h"


PeriodicTimer::PeriodicTimer(Clock::duration taskPeriod, OverrunPolicy overrunPolicy)
    : period(taskPeriod),
      scheduledStart(Clock::now()),
      policy(overrunPolicy)
{
}

