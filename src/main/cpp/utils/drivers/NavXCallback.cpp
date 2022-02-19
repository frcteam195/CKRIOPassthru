#include "utils/drivers/NavXCallback.hpp"
#include <mutex>

NavXCallback::NavXCallback(CKNavX *navX)
{
    mNavX = navX;
}

void NavXCallback::timestampedDataReceived(long system_timestamp, long sensor_timestamp, AHRSProtocol::AHRSUpdateBase& sensor_data, void * context)
{
    std::scoped_lock<std::mutex>lock(mNavX->mSyncLock);
    // // This handles the fact that the sensor is inverted from our coordinate conventions.
    if (mNavX->mLastSensorTimestampMs != CKNavX::kInvalidTimestamp && mNavX->mLastSensorTimestampMs < sensor_timestamp)
    {
        mNavX->mYawRateDegreesPerSecond = 1000.0 * (-mNavX->mYawDegrees - sensor_data.yaw) / (double) (sensor_timestamp - mNavX->mLastSensorTimestampMs);
    }
    mNavX->mLastSensorTimestampMs = sensor_timestamp;
    mNavX->mYawDegrees = -sensor_data.yaw;
    mNavX->mFusedHeading = -sensor_data.fused_heading;
}
