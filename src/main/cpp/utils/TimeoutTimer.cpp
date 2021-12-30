#include "utils/TimeoutTimer.hpp"

#include <cmath>

TimeoutTimer::TimeoutTimer(double timeoutMs)
{
    this->timeoutMs = timeoutMs;
    setFirstRun(true);
}

bool TimeoutTimer::isTimedOut()
{
    if (firstRun) {
        taskTimer.start();
        setFirstRun(false);
    }
    return taskTimer.hasElapsedMs() > timeoutMs;
}

void TimeoutTimer::reset()
{
    setFirstRun(true);
}

double TimeoutTimer::getTimeLeft()
{
    return std::fmax(timeoutMs - taskTimer.hasElapsedMs(), 0.0);
}

double TimeoutTimer::getTimeoutPeriod() const
{
    return timeoutMs;
}

void TimeoutTimer::setFirstRun(bool firstRun)
{
    std::lock_guard<std::mutex> lock(mtx);
    this->firstRun = firstRun;
}