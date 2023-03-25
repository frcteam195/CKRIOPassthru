#pragma once

#include "utils/Singleton.hpp"
#include <atomic>
#include <mutex>
#include <thread>
#include "frc/DriverStation.h"
#include <string>
#include "utils/ThreadRateControl.hpp"
#include "utils/CachedValue.hpp"
#include "utils/TimeoutTimer.hpp"

class RobotDataHelper : public Singleton<RobotDataHelper>
{
    friend Singleton;
public:
    frc::DriverStation::Alliance getAlliance();
    float getMatchTime();
    std::string getGameSpecificMsg();
    
private:
    RobotDataHelper();
    ~RobotDataHelper();
    void runThread();

    std::atomic_bool mThreadActive;

    CachedValue<frc::DriverStation::Alliance> mCachedAlliance;
    CachedValue<float> mCachedMatchTime;

    std::atomic<frc::DriverStation::Alliance> mAlliance {frc::DriverStation::Alliance::kInvalid};
    std::atomic<float> mMatchTime = 0;
    std::string mGameSpecificMsg  = "";
    std::atomic<int> mSelectedAuto = 0;

    std::recursive_mutex mStrMutex;

    ThreadRateControl mRateControl;
    TimeoutTimer mCacheTimerMsg;

    static constexpr int RBDATAHELPER_THREAD_RATE_MS = 100;

    std::thread mThread;
};
