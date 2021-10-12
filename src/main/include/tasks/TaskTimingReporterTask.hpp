#pragma once

#include <vector>
#include "Task.hpp"
#include <stdint.h>

class TaskTimingReporterTask : public Task
{
public:
    inline const static std::string TASK_NAME = "TaskTimingReporterTask";

    TaskTimingReporterTask();
    void run(unsigned int timeSinceLastUpdateMs) override;
    static constexpr uint32_t THREAD_RATE_MS = 5000;
private:
};