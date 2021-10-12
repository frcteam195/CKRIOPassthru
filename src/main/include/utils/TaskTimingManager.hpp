#pragma once

#include "utils/Singleton.hpp"
#include <map>
#include <mutex>
#include <string>

class TaskTimingManager : public Singleton<TaskTimingManager>
{
    friend Singleton;
public:
    void reportTaskTiming(std::string taskName, float value);
    void displayTaskTiming();
private:
    TaskTimingManager();
    std::map<std::string, float> mTaskTimingList;

    std::mutex taskTimingLock;
};