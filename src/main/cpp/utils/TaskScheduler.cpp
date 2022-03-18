#include "utils/TaskScheduler.hpp"
#include "utils/CKMath.hpp"
#include "units/time.h"
#include "hal/Notifier.h"
#include "hal/HAL.h"
#include "utils/SystemHelper.hpp"
#include "utils/CKErrors.hpp"

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
    t.mTaskRateMs = taskRateMs;
    taskList.push_back(&t);
}

void TaskScheduler::start()
{
    if (!mThread.joinable())
    {
        threadActive = true;
        rtTimer.start();
        mThread = std::thread(std::bind(&TaskScheduler::run, this));
    }
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
        try {
            timeNow = HAL_GetFPGATime(&c_status);
            nextWakeTime = std::numeric_limits<uint64_t>::max();
            for (Task *t : taskList)
            {
                if (t)
                {
                    if (t->mTaskRateMs <= 0) //Check loop rate is valid
                    {
                        continue;
                    }

                    //check that value is negative or 0, meaning we will send an update
                    if (t->timeNextUpdateuS < timeNow || t->timeNextUpdateuS == 0)
                    {
                        t->run(timeNow - t->timeLastUpdateuS);
                        t->timeLastUpdateuS = t->timeNextUpdateuS;
                        t->timeNextUpdateuS = t->timeLastUpdateuS + (t->mTaskRateMs * 1000);
                    }
                    nextWakeTime = ck::math::min(t->timeNextUpdateuS, nextWakeTime);
                }
            }
            HAL_UpdateNotifierAlarm(m_notifier, nextWakeTime, &c_status);
            uint64_t sleepTime = HAL_WaitForNotifierAlarm(m_notifier, &c_status) - timeNow;
            (void)sleepTime; //Remove unused variable warning
        }
        catch (std::exception& e)
        {
            ck::ReportError(e.what());
        }
        catch (...)
        {
            ck::ReportError("Unknown Exception in Task Scheduler");
        }
    }
}