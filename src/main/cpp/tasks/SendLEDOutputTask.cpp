#include "tasks/SendLEDOutputTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "LEDManager.hpp"

SendLEDOutputTask::SendLEDOutputTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void SendLEDOutputTask::run(uint32_t timeSinceLastUpdateMs)
{
    LEDManager::getInstance().sendUpdate();
    mTaskTimer.reportElapsedTime();
}