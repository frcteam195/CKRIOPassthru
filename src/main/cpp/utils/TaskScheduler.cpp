#include "utils/TaskScheduler.hpp"
#include "utils/CKMath.hpp"
#include "units/time.h"
#include "hal/Notifier.h"
#include "hal/HAL.h"
#include "utils/ThreadPriorityHelper.hpp"

#define DEBUG

TaskScheduler::TaskScheduler()
{
    m_notifier = HAL_InitializeNotifier(&c_status);
}

TaskScheduler::~TaskScheduler()
{
    stop();
}

void TaskScheduler::scheduleTask(Task &t)
{
    taskList.push_back(&t);
}

void TaskScheduler::scheduleTask(Task &t, uint32_t taskRateMs)
{
    t.taskRateMs = taskRateMs;
    taskList.push_back(&t);
}

void TaskScheduler::start()
{
    threadActive = true;
    rtTimer.start();
    mThread = std::thread(std::bind(&TaskScheduler::run, this));
}

void TaskScheduler::stop()
{
    HAL_StopNotifier(m_notifier, &c_status);
    HAL_CleanNotifier(m_notifier, &c_status);
    threadActive = false;
    if (mThread.joinable())
    {
        mThread.join();
    }
}

void TaskScheduler::run()
{
    ck::configureThreadPriority(90);
    while (threadActive)
    {
        timeNow = HAL_GetFPGATime(&c_status);
        for (Task *t : taskList)
        {
            if (t)
            {
                if (t->taskRateMs <= 0) //Check loop rate is valid
                {
                    continue;
                }

                //check that value is negative or 0, meaning we will send an update
                if (t->timeNextUpdateuS < timeNow || t->timeNextUpdateuS == 0)
                {
                    t->run(timeNow - t->timeLastUpdateuS);
                    t->timeLastUpdateuS = t->timeNextUpdateuS;
                    t->timeNextUpdateuS = t->timeLastUpdateuS + (t->taskRateMs * 1000);
                }
                nextWakeTime = nextWakeTime < timeNow ? t->timeNextUpdateuS : nextWakeTime;
                nextWakeTime = ck::math::min(t->timeNextUpdateuS, nextWakeTime);
            }
        }
        HAL_UpdateNotifierAlarm(m_notifier, nextWakeTime, &c_status);
        uint64_t sleepTime = HAL_WaitForNotifierAlarm(m_notifier, &c_status) - timeNow;
        sleepTime = 0;
    }
}