#include "utils/TaskScheduler.hpp"
#include "utils/CKMath.hpp"
#include "units/time.h"

TaskScheduler::TaskScheduler() : taskThreadNotifier(std::bind(&TaskScheduler::run, this))
{
}

TaskScheduler::~TaskScheduler()
{
    stop();
}

void TaskScheduler::scheduleTask(Task t)
{
    taskList.push_back(t);
}

void TaskScheduler::start()
{
    eTimer.start();
    taskThreadNotifier.StartPeriodic((units::second_t)0.0005);
}

void TaskScheduler::stop()
{
    taskThreadNotifier.Stop();
}

void TaskScheduler::run()
{
    //Timing based on previous work from here: https://github.com/guitar24t/SPARK-MAX-roboRIO/blob/master/src/main/driver/cpp/CANSparkMaxSetDriver.cpp
    //TODO: Consider converting this to use HAL_Notifier and do own thread management instead of using Notifier
    for (Task t : taskList)
    {
        if (t.taskRateSec <= 0) //Check loop rate is valid
        {
            continue;
        }

        timeNow = eTimer.hasElapsed();
        t.timeNextUpdate = t.timeLastUpdate + t.taskRateSec;

        //check that value is negative, meaning we will send an update
        if ((t.timeNextUpdate - timeNow) > 0)
        {
            nextWakeTime = ck::math::min(t.timeNextUpdate, nextWakeTime);
        }
        else
        {
            t.funcPtr((timeNow - t.timeLastUpdate) * 1000);
            t.timeLastUpdate = eTimer.hasElapsed();
        }
    }
}