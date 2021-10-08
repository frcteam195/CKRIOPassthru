#pragma once

#include "Singleton.hpp"
#include <vector>
#include <functional>
#include <frc/Notifier.h>
#include "utils/RTTimer.hpp"
#include <stdint.h>
#include <thread>
#include "tasks/Task.hpp"

class TaskScheduler : public Singleton<TaskScheduler>
{
    friend Singleton;
public:
    void scheduleTask(Task &t);
    void scheduleTask(Task &t, uint32_t taskRateMs);
    void start();
    void stop();
private:
    TaskScheduler();
    ~TaskScheduler();
    void run();
    RTTimer rtTimer;
    bool threadActive;
    std::thread mThread;
    uint64_t timeNow;
    uint64_t nextWakeTime = std::numeric_limits<uint64_t>::max();
    std::vector<Task*> taskList;
    int32_t c_status;
    uint32_t m_notifier;
};