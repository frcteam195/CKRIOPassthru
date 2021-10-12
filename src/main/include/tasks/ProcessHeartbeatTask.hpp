#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>

class ProcessHeartbeatTask : public Task
{
public:
    ProcessHeartbeatTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 20;
private:
};