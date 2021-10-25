#pragma once

#include <vector>
#include "Task.hpp"
#include "utils/CKLogger.hpp"

class CKLoggerTask : public Task
{
public:
    inline const static std::string TASK_NAME = "CKLoggerTask";

    CKLoggerTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 1000;
private:
};