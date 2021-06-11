#pragma once

#include <vector>
#include "DataStructures.hpp"
#include "Task.hpp"
#include <stdint.h>

class SendMotorConfigTask : public Task
{
public:
    SendMotorConfigTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
private:
    static constexpr uint32_t THREAD_RATE_MS = 100;
};