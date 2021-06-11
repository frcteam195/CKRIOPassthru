#include "utils/RTTimer.hpp"
#include "hal/HAL.h"

RTTimer::RTTimer()
{
    start();
}

void RTTimer::start()
{
    int32_t status;
    startTimeMs = HAL_GetFPGATime(&status) / 1000;
}

uint32_t RTTimer::hasElapsedMs()
{
    int32_t status;
    return ((uint32_t)(HAL_GetFPGATime(&status)/1000)) - startTimeMs;
}
