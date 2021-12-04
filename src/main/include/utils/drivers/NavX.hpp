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
    double getRoll();
    double getPitch();
    double getYawRateDegreesPerSec();
    double getFusedHeading();

	double mYawDegrees;
	double mFusedHeading;
	double mYawRateDegreesPerSecond;
	static constexpr long kInvalidTimestamp = -1;
	long mLastSensorTimestampMs;

    std::mutex mSyncLock;

protected:
    AHRS mAHRS;
    NavXCallback *mNavXCallback;

private:
    void resetState();
};
