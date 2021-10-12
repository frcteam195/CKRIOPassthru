#pragma once

#include <functional>
#include <stdint.h>
#include "utils/TaskTimer.hpp"
#include <string>

class Task {
public:
    uint32_t mTaskRateMs;
    uint64_t timeNextUpdateuS;
    uint64_t timeLastUpdateuS;
    Task(uint32_t taskRateMs, std::string taskName) : mTaskRateMs(taskRateMs), mTaskTimer(taskName) {};
    virtual void run(unsigned int timeSinceLastUpdateMs) = 0;
protected:
    TaskTimer mTaskTimer;
private:
    Task() = delete;
};