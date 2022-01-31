#include "utils/ThreadRateControl.hpp"
#include "hal/HAL.h"

ThreadRateControl::ThreadRateControl() : m_notifier(HAL_InitializeNotifier(&m_notifierStatus)), eTimer() {}

ThreadRateControl::~ThreadRateControl() {
    HAL_StopNotifier((HAL_NotifierHandle)m_notifier, &m_notifierStatus);
    HAL_CleanNotifier((HAL_NotifierHandle)m_notifier, &m_notifierStatus);
}

void ThreadRateControl::start() {
    if (!started)
    {
        eTimer.start();
        started = true;
    }
}

void ThreadRateControl::start(bool resetStart) {
    if (resetStart)
    {
        started = false;
    }
    start();
}

void ThreadRateControl::doRateControl(int minLoopTimeMs) {
    double remainingTimeMs = minLoopTimeMs - eTimer.hasElapsedMs();
    if (remainingTimeMs > 0) {
        //Subtract constant offset for code delay (currently 150us)
        int status;
        HAL_UpdateNotifierAlarm((HAL_NotifierHandle)m_notifier, static_cast<uint64_t>((HAL_GetFPGATime(&status) + (remainingTimeMs * 1.0e3)) - 150), &m_notifierStatus);
        static_cast<void>(HAL_WaitForNotifierAlarm((HAL_NotifierHandle)m_notifier, &m_notifierStatus)); //Intentionally discard no_discard result
    }
    eTimer.start();
}