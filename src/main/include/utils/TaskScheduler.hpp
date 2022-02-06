#pragma once

#include "Singleton.hpp"
#include <vector>
#include <functional>
#include <frc/Notifier.h>
#include "utils/RTTimer.hpp"
#include <stdint.h>
#include <thread>
#include "tasks/Task.hpp"
#include <atomic>

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
    std::atomic_bool threadActive = false;
    std::thread mThread;
    uint64_t timeNow = 0;
    uint64_t nextWakeTime = std::numeric_limits<uint64_t>::max();
    std::vector<Task*> taskList;
    int32_t c_status = 0;
    uint32_t m_notifier = 0;
};