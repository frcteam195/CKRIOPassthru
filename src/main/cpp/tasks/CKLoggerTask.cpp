#include "tasks/CKLoggerTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "utils/GlobalConfig.hpp"

CKLoggerTask::CKLoggerTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void CKLoggerTask::run(uint32_t timeSinceLastUpdateMs)
{
#ifdef CONSOLE_REPORTING
    ckLogger.messagePump();
    mTaskTimer.reportElapsedTime();
#endif
}