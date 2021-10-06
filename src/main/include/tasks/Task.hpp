#pragma once
#include <functional>
#include <stdint.h>

class Task {
public:
    uint32_t taskRateMs;
    uint32_t timeNextUpdateMs;
    uint32_t timeLastUpdateMs;
    Task(uint32_t taskRateMs) : taskRateMs(taskRateMs) {};
    virtual void run(unsigned int timeSinceLastUpdateMs) = 0;
private:
    Task() = delete;
};