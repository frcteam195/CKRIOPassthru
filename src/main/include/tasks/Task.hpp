#pragma once

#include <functional>
#include <stdint.h>

class Task {
public:
    uint32_t taskRateMs;
    uint64_t timeNextUpdateuS;
    uint64_t timeLastUpdateuS;
    Task(uint32_t taskRateMs) : taskRateMs(taskRateMs) {};
    virtual void run(unsigned int timeSinceLastUpdateMs) = 0;
private:
    Task() = delete;
};