#include "tasks/SendOutboundDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>

SendOutboundDataTask::SendOutboundDataTask() : Task(THREAD_RATE_MS) {}

void SendOutboundDataTask::run(uint32_t timeSinceLastUpdateMs)
{
}