#include "tasks/ReadSensorDataTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>

ReadSensorDataTask::ReadSensorDataTask() : Task(THREAD_RATE_MS) {}

void ReadSensorDataTask::run(uint32_t timeSinceLastUpdateMs)
{
    std::cout << "Actual 5ms loop time (ms): " << (rtTimer.hasElapseduS() / 1000.0) << std::endl;
    rtTimer.start();
}