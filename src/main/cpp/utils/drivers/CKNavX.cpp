#include "utils/drivers/CKNavX.hpp"
#include "utils/TimeoutTimer.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include "utils/CKMath.hpp"

CKNavX::CKNavX() : CKNavX(frc::SPI::kMXP) {
    
}

CKNavX::CKNavX(frc::SPI::Port spi_port_id) : mAHRS(spi_port_id, 200) {
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

CKNavX::~CKNavX() {
    delete mNavXCallback;
}

bool CKNavX::setYaw(double yaw)
{
    zeroYaw();
    return true;
}

bool CKNavX::getQuaternion(double quaternion[4])
{
    quaternion[0] = mAHRS.GetQuaternionW();
    quaternion[1] = mAHRS.GetQuaternionX();
    quaternion[2] = mAHRS.GetQuaternionY();
    quaternion[3] = mAHRS.GetQuaternionZ();
    return true;
}

bool CKNavX::getYPR(double ypr[3])
{
    ypr[0] = getFusedHeadingRad();
    ypr[1] = getPitchRad();
    ypr[2] = getRollRad();
    return true;
}

bool CKNavX::configMountPose(AxisDirection forward, AxisDirection up)
{
    return false;
}

bool CKNavX::isPresent() {
    return mAHRS.IsConnected();
}

bool CKNavX::reset() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    mAHRS.Reset();
    resetState();
	return true;
}

void CKNavX::zeroYaw() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    mAHRS.ZeroYaw();
	resetState();
}

void CKNavX::resetState() {
    mLastSensorTimestampMs = kInvalidTimestamp;
    mFusedHeading = 0.0;
    mYawDegrees = 0.0;
    mYawRateDegreesPerSecond = 0.0;
}

double CKNavX::getYaw() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mYawDegrees;
}

double CKNavX::getRawYaw() {
    return mAHRS.GetYaw();
}

double CKNavX::getRoll() {
    return mAHRS.GetRoll();
}

double CKNavX::getRollRad() {
    return ck::math::deg2rad(mAHRS.GetRoll());
}

double CKNavX::getPitch() {
    return mAHRS.GetPitch();
}

double CKNavX::getPitchRad() {
    return ck::math::deg2rad(mAHRS.GetPitch());
}

double CKNavX::getYawRateRadPerSec() {
    return ck::math::deg2rad(getYawRateDegreesPerSec());
}

double CKNavX::getYawRateDegreesPerSec() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mYawRateDegreesPerSecond;
}

double CKNavX::getFusedHeading() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mFusedHeading;
}

double CKNavX::getFusedHeadingRad() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return ck::math::deg2rad(mFusedHeading);
}

bool CKNavX::hasUpdated() {
    long currAHRSTimestamp = mLastSensorTimestampMs;
    if (mRawSensorTimestampPrev < currAHRSTimestamp)
    {
        mRawSensorTimestampPrev = currAHRSTimestamp;
        return true;
    }
    return false;
}