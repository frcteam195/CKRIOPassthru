#include "utils/TaskTimingManager.hpp"
#include <iostream>
#include "utils/GlobalConfig.hpp"
#include "utils/CKLogger.hpp"

TaskTimingManager::TaskTimingManager() {}

void TaskTimingManager::reportTaskTiming(std::string taskName, float value)
{
#ifdef ENABLE_TASK_TIME_REPORTING
    std::scoped_lock<std::mutex> lock(taskTimingLock);
    mTaskTimingList[taskName] = value;
#endif
}

void TaskTimingManager::displayTaskTiming()
{
#ifdef ENABLE_TASK_TIME_REPORTING
    std::scoped_lock<std::mutex> lock(taskTimingLock);
    for (auto const& [key, val] : mTaskTimingList)
    {
        std::cout << key << ": " << val << "ms" << std::endl;
    }
    std::cout << std::endl;
#endif
}
