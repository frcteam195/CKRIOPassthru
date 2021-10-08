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
    // int xRun = 0;
    while (threadActive)
    {
        timeNow = HAL_GetFPGATime(&c_status);
        //Timing based on previous work from here: https://github.com/guitar24t/SPARK-MAX-roboRIO/blob/master/src/main/driver/cpp/CANSparkMaxSetDriver.cpp
        //TODO: Consider converting this to use HAL_Notifier and do own thread management instead of using Notifier
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
                    t->timeLastUpdateuS = timeNow;
                    t->timeNextUpdateuS = t->timeLastUpdateuS + (t->taskRateMs * 1000);
                    nextWakeTime = nextWakeTime < timeNow ? t->timeNextUpdateuS : nextWakeTime;
                    nextWakeTime = ck::math::min(t->timeNextUpdateuS, nextWakeTime);

                    // if (xRun++ % 50 == 0) {
                    //     std::cout << "Time now: " << timeNow << std::endl;
                    //     std::cout << "Next update: " << t->timeNextUpdateuS << std::endl;
                    //     std::cout << "Last update: " << t->timeLastUpdateuS << std::endl;
                    //     std::cout << "Next wake time: " << nextWakeTime << std::endl;
                    // }
                }
            }
        }
        //TODO: Test this timing, this doesn't likely work yet.
        HAL_UpdateNotifierAlarm(m_notifier, nextWakeTime, &c_status);
        uint64_t sleepTime = HAL_WaitForNotifierAlarm(m_notifier, &c_status) - timeNow;
        // if (xRun % 50 == 0) {
        //     std::cout << "Actual sleep time: " << sleepTime << std::endl;
        // }
        sleepTime = 0;
    }
}