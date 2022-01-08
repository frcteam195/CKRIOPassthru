#pragma once

#include <AHRS.h>
#include <mutex>
#include "NavXCallback.hpp"

class NavXCallback;

class NavX {
public:
    NavX();
    ~NavX();
    NavX(frc::SPI::Port spi_port_id);

    bool isPresent();
    bool reset();
    void zeroYaw();
    double getYaw();
    double getRawYaw();
    double getRoll();
    double getRollRad();
    double getPitch();
    double getPitchRad();
    double getYawRateRadPerSec();
    double getYawRateDegreesPerSec();
    double getFusedHeading();
    double getFusedHeadingRad();
    bool hasUpdated();

	double mYawDegrees;
	double mFusedHeading;
	double mYawRateDegreesPerSecond;
	static constexpr long kInvalidTimestamp = -1;
	long mLastSensorTimestampMs;
    
    long mRawSensorTimestampPrev = -1;  //Init to -1 to force an update the first time

    std::mutex mSyncLock;

protected:
    AHRS mAHRS;
    NavXCallback *mNavXCallback;

private:
    void resetState();
};
