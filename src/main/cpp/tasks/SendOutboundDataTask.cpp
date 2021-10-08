#include "tasks/SendOutboundDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>


SendOutboundDataTask::SendOutboundDataTask() : Task(THREAD_RATE_MS)
{
    rtTimer.start();
}

void SendOutboundDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    std::cout << "Actual 10ms loop time (ms): " << (rtTimer.hasElapseduS() / 1000.0) << std::endl;
    rtTimer.start();
}