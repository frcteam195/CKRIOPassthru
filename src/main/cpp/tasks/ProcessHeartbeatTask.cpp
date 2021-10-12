#include "tasks/ProcessHeartbeatTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "MotorManager.hpp"

ProcessHeartbeatTask::ProcessHeartbeatTask() : Task(THREAD_RATE_MS) {}
void ProcessHeartbeatTask::run(uint32_t timeSinceLastUpdateMs)
{
    MotorManager::getInstance().processHeartbeat();;
}