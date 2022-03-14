#include "utils/TaskTimer.hpp"
#include "hal/HAL.h"
#include <iostream>
#include "utils/GlobalConfig.hpp"

TaskTimer::TaskTimer()
{
    TaskTimer("");
}

TaskTimer::TaskTimer(std::string taskName) : mTaskName(taskName)
{
    start();
}

void TaskTimer::start()
{
    int32_t status;
    startTimeuS = HAL_GetFPGATime(&status);
#ifdef DEBUG
    // std::cout << "Start running " << mTaskName << std::endl;
#endif
}

float TaskTimer::hasElapsedMs()
{
    int32_t status;
    return (HAL_GetFPGATime(&status) - startTimeuS) * 0.001f;
}

void TaskTimer::reportElapsedTime()
{
#ifdef ENABLE_TASK_TIME_REPORTING
    TaskTimingManager::getInstance().reportTaskTiming(mTaskName, hasElapsedMs());
    start();
#endif
}