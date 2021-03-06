#include "RobotDataHelper.hpp"
#include "utils/RobotControlModeHelper.hpp"
#include <iostream>
#include "utils/CKErrors.hpp"

RobotDataHelper::RobotDataHelper() :
mThreadActive(true),
mThread(&RobotDataHelper::runThread, this),
mCachedAlliance(250, []() {
    if (RobotControlModeHelper::getInstance().isDSAttached())
    {
        return frc::DriverStation::GetAlliance();
    }
    else
    {
        return frc::DriverStation::Alliance::kInvalid;
    }
} ),
mCachedMatchTime(500, []() {
    if (RobotControlModeHelper::getInstance().isDSAttached())
    {
        return (float)frc::DriverStation::GetMatchTime();
    }
    else
    {
        return -1.0f;
    }
} ),
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
        try {
            //mAlliance = mCachedAlliance.getValue();
            mAlliance = frc::DriverStation::GetAlliance();
            mMatchTime = (float)frc::DriverStation::GetMatchTime();
            // mMatchTime = mCachedMatchTime.getValue();
            mGameSpecificMsg = "";
            // TODO: Figure out why this is causing crash when DS not connected
            // if (mCacheTimerMsg.isTimedOut())
            // {
            //     std::scoped_lock<std::recursive_mutex> lock(mStrMutex);
            //     mGameSpecificMsg = frc::DriverStation::GetGameSpecificMessage();
            //     mCacheTimerMsg.reset();
            // }
        }
        catch (std::exception& e)
        {
            mAlliance = frc::DriverStation::Alliance::kInvalid;
            mMatchTime = -1;
            mGameSpecificMsg = "";
            ck::ReportError("Failed to get DS data");
            ck::ReportError(e.what());
        }
        catch (...)
        {
            ck::ReportError("Unknown Exception from DS Data");
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


void RobotDataHelper::setSelectedAuto(int selectedAuto)
{
    mSelectedAuto = selectedAuto;
}

int RobotDataHelper::getSelectedAuto()
{
    return mSelectedAuto;
}