#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>

class ReceiveMessagesTask : public Task
{
public:
    inline const static std::string TASK_NAME = "ReceiveMessagesTask";

    ReceiveMessagesTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 2;
private:
};