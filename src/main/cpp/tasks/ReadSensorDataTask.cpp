#include "tasks/ReadSensorDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>

ReadSensorDataTask::ReadSensorDataTask() : Task(THREAD_RATE_MS) {}

void ReadSensorDataTask::run(uint32_t timeSinceLastUpdateMs)
{
}