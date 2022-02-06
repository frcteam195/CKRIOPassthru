#pragma once

#include "TaskTimer.hpp"

class TimeoutTimer {
public:
    TimeoutTimer(double timeoutMs);
    bool isTimedOut();
    void reset();
    double getTimeLeft();
    double getTimeoutPeriod() const;

private:
    double timeoutMs = 0;
    bool firstRun = true;
    TaskTimer taskTimer;

    std::mutex mtx; 

    void setFirstRun(bool firstRun);
};