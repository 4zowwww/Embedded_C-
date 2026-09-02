#include "Watchdog.h"





Watchdog::Watchdog(Clock::duration watchdogTimeout) :
    timeout(watchdogTimeout),
    lastKick(Clock::now())
    {
    }

void Watchdog::kick() {

    std::lock_guard<std::mutex> lock(mtx);

    lastKick = Clock::now();
}

bool Watchdog::expired() const {

    std::lock_guard<std::mutex> lock(mtx);

    return Clock::now() - lastKick > timeout;
}

