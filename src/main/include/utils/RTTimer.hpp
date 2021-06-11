#pragma once

#include "frc/Timer.h"
#include <stdint.h>

class RTTimer {
public:
    RTTimer();
    void start();
    uint32_t hasElapsedMs();
private:
    uint32_t startTimeMs;
};
