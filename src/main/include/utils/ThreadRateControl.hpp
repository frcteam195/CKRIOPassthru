#pragma once

#include "TaskTimer.hpp"
#include "hal/Notifier.h"

class ThreadRateControl {
public:
    ThreadRateControl();
    ~ThreadRateControl();
    void start();
    void start(bool resetStart);
    void doRateControl(int minLoopTimeMs);

private:
    const HAL_NotifierHandle m_notifier;
    TaskTimer eTimer;
    double prevDtCalcTime = 0;
    bool started = false;
    int m_notifierStatus = 0;

};