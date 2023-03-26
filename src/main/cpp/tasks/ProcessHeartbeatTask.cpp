#include "tasks/ProcessHeartbeatTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "MotorManager.hpp"
#include "CANdleManager.hpp"
#include "ExternalControlManager.hpp"
#include "SolenoidManager.hpp"

ProcessHeartbeatTask::ProcessHeartbeatTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void ProcessHeartbeatTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    MotorManager::getInstance().processHeartbeat();
    SolenoidManager::getInstance().processHeartbeat();
    ExternalControlManager::getInstance().processHeartbeat();
    CANdleManager::getInstance().processHeartbeat();
    mTaskTimer.reportElapsedTime();
}