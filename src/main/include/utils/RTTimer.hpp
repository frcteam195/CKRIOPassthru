#pragma once

#include "frc/Timer.h"
#include <stdint.h>

class RTTimer {
public:
    RTTimer();
    void start();
    uint64_t hasElapseduS();
private:
    uint64_t startTimeuS;
};
