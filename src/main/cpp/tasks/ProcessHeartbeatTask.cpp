#include "tasks/ProcessHeartbeatTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "MotorManager.hpp"
#include "ExternalControlManager.hpp"

ProcessHeartbeatTask::ProcessHeartbeatTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void ProcessHeartbeatTask::run(uint32_t timeSinceLastUpdateMs)
{
    MotorManager::getInstance().processHeartbeat();
    ExternalControlManager::getInstance().processHeartbeat();
    mTaskTimer.reportElapsedTime();
}