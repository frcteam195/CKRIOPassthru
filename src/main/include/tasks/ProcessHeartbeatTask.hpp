#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>

class ProcessHeartbeatTask : public Task
{
public:
    inline const static std::string TASK_NAME = "ProcessHeartbeatTask";

    ProcessHeartbeatTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 20;
private:
};