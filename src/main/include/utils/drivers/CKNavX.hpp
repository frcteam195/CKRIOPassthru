#pragma once

#include <AHRS.h>
#include <mutex>
#include "NavXCallback.hpp"
#include "CKIMU.hpp"

class NavXCallback;

class CKNavX : public CKIMU
{
public:
    CKNavX();
    ~CKNavX();
    CKNavX(frc::SPI::Port spi_port_id);

    bool setYaw(double yaw) override;
    bool getQuaternion(double quaternion[4]) override;
    bool configMountPose(AxisDirection forward, AxisDirection up) override;

    bool isPresent();
    bool reset() override;
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

	double mYawDegrees = 0;
	double mFusedHeading = 0;
	double mYawRateDegreesPerSecond = 0;
	static constexpr long kInvalidTimestamp = -1;
	long mLastSensorTimestampMs = 0;
    
    long mRawSensorTimestampPrev = -1;  //Init to -1 to force an update the first time

    std::mutex mSyncLock;

protected:
    AHRS mAHRS;
    NavXCallback *mNavXCallback;

private:
    void resetState();
};
