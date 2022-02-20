#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>

class SendLEDOutputTask : public Task
{
public:
    inline const static std::string TASK_NAME = "SendLEDOutputTask";

    SendLEDOutputTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 200;
private:
};