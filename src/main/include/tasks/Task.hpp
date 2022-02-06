#pragma once

#include <functional>
#include <stdint.h>
#include "utils/TaskTimer.hpp"
#include <string>
#include "utils/GlobalConfig.hpp"

class Task {
public:
    uint32_t mTaskRateMs = 0;
    uint64_t timeNextUpdateuS = 0;
    uint64_t timeLastUpdateuS = 0;
    Task(uint32_t taskRateMs, std::string taskName) : mTaskRateMs(taskRateMs), mTaskTimer(taskName) {};
    virtual void run(unsigned int timeSinceLastUpdateMs) = 0;
protected:
    TaskTimer mTaskTimer;
private:
    Task() = delete;
};