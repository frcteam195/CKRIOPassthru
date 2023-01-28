#include "tasks/UpdateDSConnectionStatusTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "utils/DSConnectionMonitor.hpp"

UpdateDSConnectionStatusTask::UpdateDSConnectionStatusTask() : Task(THREAD_RATE_MS, TASK_NAME) {}

UpdateDSConnectionStatusTask::~UpdateDSConnectionStatusTask() {}


void UpdateDSConnectionStatusTask::run(uint32_t timeSinceLastUpdateMs)
{
    mTaskTimer.start();
    DSConnectionMonitor::getInstance().updateConnectionStatus();
    mTaskTimer.reportElapsedTime();
}