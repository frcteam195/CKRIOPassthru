#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>

class SendMotorValuesTask : public Task
{
public:
    SendMotorValuesTask();
    ~SendMotorValuesTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 5;
private:
};