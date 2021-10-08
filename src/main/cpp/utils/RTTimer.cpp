#include "utils/RTTimer.hpp"
#include "hal/HAL.h"

RTTimer::RTTimer()
{
    start();
}

void RTTimer::start()
{
    int32_t status;
    startTimeuS = HAL_GetFPGATime(&status);
}

uint64_t RTTimer::hasElapseduS()
{
    int32_t status;
    return HAL_GetFPGATime(&status) - startTimeuS;
}
