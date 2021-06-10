#include "tasks/SendMotorValuesTask.hpp"
#include "utils/TaskScheduler.hpp"
#include <functional>

SendMotorValuesTask::SendMotorValuesTask()
{
    Task t(std::bind(&SendMotorValuesTask::run, this), 0.005);
    TaskScheduler::getInstance().scheduleTask(t);
}