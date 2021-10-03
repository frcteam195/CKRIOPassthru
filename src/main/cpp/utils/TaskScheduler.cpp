#include "utils/TaskScheduler.hpp"
#include "utils/CKMath.hpp"
#include "units/time.h"
#include "hal/Notifier.h"
#include "hal/HAL.h"

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
    ///////////////////////////
    //Set Scheduler Thread Priority below main thread to prevent preempting netcomm
    sched_param sch;
    sch.sched_priority = 90;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch) != 0)
    {
        std::cout << "Error setting scheduler thread priority!" << std::endl;
    }
    ///////////////////////////

    while (threadActive)
    {
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

                timeNow = rtTimer.hasElapsedMs();
                t->timeNextUpdateMs = t->timeLastUpdateMs + t->taskRateMs;

                //check that value is negative, meaning we will send an update
                if ((t->timeNextUpdateMs - timeNow) > 0)
                {
                    nextWakeTime = ck::math::min(t->timeNextUpdateMs, nextWakeTime);
                }
                else
                {
                    t->run(timeNow - t->timeLastUpdateMs);
                    t->timeLastUpdateMs = rtTimer.hasElapsedMs();
                }
            }
        }
        //TODO: Fix this timing, this doesn't likely work yet.
        HAL_UpdateNotifierAlarm(m_notifier, HAL_GetFPGATime(&c_status) + (nextWakeTime * 1000), &c_status);
        uint64_t retTime = HAL_WaitForNotifierAlarm(m_notifier, &c_status);
    }
}