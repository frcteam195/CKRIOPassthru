#include "tasks/ReceiveMessagesTask.hpp"
#include "utils/TaskScheduler.hpp"
#include "NetworkManager.hpp"

ReceiveMessagesTask::ReceiveMessagesTask() : Task(THREAD_RATE_MS, TASK_NAME) {}
void ReceiveMessagesTask::run(uint32_t timeSinceLastUpdateMs)
{
    NetworkManager::getInstance().receiveMessagePump();
    mTaskTimer.reportElapsedTime();
}