#pragma once

#include "Singleton.hpp"
#include <vector>
#include <functional>
#include <frc/Notifier.h>
#include "utils/ElapsedTimer.hpp"

//void func(uint32_t elapsedTimeMSSinceLastCall)
#define FUNC_TYPE std::function<void(unsigned int)>

struct Task {
    FUNC_TYPE funcPtr;
    double taskRateSec;
    double timeNextUpdate;
    double timeLastUpdate;
public:
    Task(FUNC_TYPE funcPtr, double taskRateSec) : funcPtr(funcPtr), taskRateSec(taskRateSec) {};
};

class TaskScheduler : public Singleton<TaskScheduler>
{
    friend Singleton;
public:
    void scheduleTask(Task t);
    void start();
    void stop();
private:
    TaskScheduler();
    ~TaskScheduler();
    void run();
    ElapsedTimer eTimer;
    double timeNow;
    double nextWakeTime;
    std::vector<Task> taskList;
    frc::Notifier taskThreadNotifier;
};