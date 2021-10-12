#include "tasks/TaskTimingReporterTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "MotorManager.hpp"

TaskTimingReporterTask::TaskTimingReporterTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void TaskTimingReporterTask::run(uint32_t timeSinceLastUpdateMs)
{
    TaskTimingManager::getInstance().displayTaskTiming();
    mTaskTimer.reportElapsedTime();
}