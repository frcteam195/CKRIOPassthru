#include "utils/drivers/NavX.hpp"

NavX::NavX() : NavX(frc::SPI::kMXP) {
    
}

NavX::NavX(frc::SPI::Port spi_port_id) : mAHRS(spi_port_id, 200) {
    resetState();
    mNavXCallback = new NavXCallback(this);
    mAHRS.RegisterCallback(mNavXCallback, nullptr);
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
    mYawDegrees = 0.0;
    mYawRateDegreesPerSecond = 0.0;
}

double NavX::getYaw() {
    std::scoped_lock<std::mutex>lock(mSyncLock);
    return mYawDegrees;
}

double NavX::getRoll() {
    return mAHRS.GetRoll();
}

double NavX::getPitch() {
    return mAHRS.GetPitch();
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
    long currAHRSTimestamp = mAHRS.GetLastSensorTimestamp();
    if (mRawSensorTimestampPrev < currAHRSTimestamp)
    {
        mRawSensorTimestampPrev = currAHRSTimestamp;
        return true;
    }
    return false;
}