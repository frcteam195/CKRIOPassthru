#include "utils/TaskTimer.hpp"
#include "hal/HAL.h"
#include <iostream>

TaskTimer::TaskTimer(std::string taskName) : mTaskName(taskName)
{
    start();
}

void TaskTimer::start()
{
    int32_t status;
    startTimeuS = HAL_GetFPGATime(&status);
}

float TaskTimer::hasElapsedMs()
{
    int32_t status;
    return (HAL_GetFPGATime(&status) - startTimeuS) * 0.001f;
}

void TaskTimer::reportElapsedTime()
{
    TaskTimingManager::getInstance().reportTaskTiming(mTaskName, hasElapsedMs());
    start();
}