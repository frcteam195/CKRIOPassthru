#include "RobotDataHelper.hpp"
#include <iostream>

RobotDataHelper::RobotDataHelper() :
mThreadActive(true),
mThread(&RobotDataHelper::runThread, this),
mCachedAlliance(250, []() { return frc::DriverStation::GetAlliance(); } ),
mCachedMatchTime(500, []() { return frc::DriverStation::GetMatchTime(); } ),
mCacheTimerMsg(250)
{}

RobotDataHelper::~RobotDataHelper()
{
    mThreadActive = false;
    if (mThread.joinable())
    {
        mThread.join();
    }
}

void RobotDataHelper::runThread()
{
    mRateControl.start();
    while (mThreadActive)
    {
        mAlliance = mCachedAlliance.getValue();
        mMatchTime = mCachedMatchTime.getValue();
        if (mCacheTimerMsg.isTimedOut())
        {
            std::scoped_lock<std::recursive_mutex> lock(mStrMutex);
            mGameSpecificMsg = frc::DriverStation::GetGameSpecificMessage();
            mCacheTimerMsg.reset();
        }
        mRateControl.doRateControl(RBDATAHELPER_THREAD_RATE_MS);
    }
}

frc::DriverStation::Alliance RobotDataHelper::getAlliance()
{
    return mAlliance;
}

float RobotDataHelper::getMatchTime()
{
    return mMatchTime;
}

std::string RobotDataHelper::getGameSpecificMsg()
{
    std::scoped_lock<std::recursive_mutex> lock(mStrMutex);
    return mGameSpecificMsg;
}
    