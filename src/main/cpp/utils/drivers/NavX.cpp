#include "utils/drivers/NavX.hpp"
#include "utils/TimeoutTimer.hpp"
#include <chrono>
#include <thread>

NavX::NavX() : NavX(frc::SPI::kMXP) {
    
}

NavX::NavX(frc::SPI::Port spi_port_id) : mAHRS(spi_port_id, 200) {
    resetState();
    mNavXCallback = new NavXCallback(this);
    mAHRS.RegisterCallback(mNavXCallback, nullptr);
    int count = 0;
    TimeoutTimer timeoutTimer(3000);
    while ((!isPresent() || mAHRS.IsCalibrating()) && !timeoutTimer.isTimedOut())
    {
        if (count++ % 5 == 0)
        {
            std::cout << "Waiting for NavX Init and Calibration..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    if (timeoutTimer.isTimedOut())
    {
        std::cout << "NavX has failed to init in allotted time." << std::endl;
    }
    else
    {
        std::cout << "Finished NavX Init! Zeroing Yaw..." << std::endl;
    }
    
    //This is lousy, but it seems to be the only way to ensure the navx is zero'd on boot
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    zeroYaw();
}

NavX::~NavX() {
    delete mNavXCallback;
}

bool NavX::isPresent() {
    return mAHRS.IsConnected();
}

bool NavX::reset() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    mAHRS.Reset();
    resetState();
	return true;
}

void NavX::zeroYaw() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    mAHRS.ZeroYaw();
	resetState();
}

void NavX::resetState() {
    mLastSensorTimestampMs = kInvalidTimestamp;
    mFusedHeading = 0.0;
    mYawDegrees = 0.0;
    mYawRateDegreesPerSecond = 0.0;
}

double NavX::getYaw() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mYawDegrees;
}

double NavX::getRawYaw() {
    return mAHRS.GetYaw();
}

double NavX::getRoll() {
    return mAHRS.GetRoll();
}

double NavX::getPitch() {
    return mAHRS.GetPitch();
}

double NavX::getYawRateRadPerSec() {
    return getYawRateDegreesPerSec() / 180 * M_PI;
}

double NavX::getYawRateDegreesPerSec() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mYawRateDegreesPerSecond;
}

double NavX::getFusedHeading() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mFusedHeading;
}

bool NavX::hasUpdated() {
    long currAHRSTimestamp = mLastSensorTimestampMs;
    if (mRawSensorTimestampPrev < currAHRSTimestamp)
    {
        mRawSensorTimestampPrev = currAHRSTimestamp;
        return true;
    }
    return false;
}