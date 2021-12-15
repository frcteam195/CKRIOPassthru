#pragma once

#include <stdint.h>
#include <string>
#include "TaskTimingManager.hpp"

class TaskTimer {
public:
    TaskTimer();
    TaskTimer(std::string taskName);
    void start();
    float hasElapsedMs();
    void reportElapsedTime();
private:
    uint64_t startTimeuS;
    std::string mTaskName;
};
