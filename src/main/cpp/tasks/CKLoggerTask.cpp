#include "tasks/CKLoggerTask.hpp"
#include "utils/TaskScheduler.hpp"

CKLoggerTask::CKLoggerTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void CKLoggerTask::run(uint32_t timeSinceLastUpdateMs)
{
    ckLogger.messagePump();
    mTaskTimer.reportElapsedTime();
}