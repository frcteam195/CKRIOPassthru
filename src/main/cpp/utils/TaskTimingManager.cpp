#include "utils/TaskTimingManager.hpp"
#include <iostream>

TaskTimingManager::TaskTimingManager() {}

void TaskTimingManager::reportTaskTiming(std::string taskName, float value)
{
    std::scoped_lock<std::mutex> lock(taskTimingLock);
    mTaskTimingList[taskName] = value;
}

void TaskTimingManager::displayTaskTiming()
{
    std::scoped_lock<std::mutex> lock(taskTimingLock);
    for (auto const& [key, val] : mTaskTimingList)
    {
        std::cout << key << ": " << val << "ms" << std::endl;
    }
    std::cout << std::endl;
}
