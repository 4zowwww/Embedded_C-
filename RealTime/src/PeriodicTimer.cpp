#include "PeriodicTimer.h"


PeriodicTimer::PeriodicTimer(Clock::duration taskPeriod)
    : period(taskPeriod),
      scheduledStart(Clock::now())
{
}