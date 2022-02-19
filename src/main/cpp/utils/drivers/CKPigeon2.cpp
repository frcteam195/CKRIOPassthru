#include "utils/drivers/CKPigeon2.hpp"
#include "utils/PhoenixHelper.hpp"

CKPigeon2::CKPigeon2(int deviceNumber, std::string const &canbus)
: mPigeon(deviceNumber, canbus)
{}

bool CKPigeon2::setYaw(double yaw)
{
    return ck::runPhoenixFunctionWithRetry([&]() { return (ErrorCode)mPigeon.SetYaw(yaw, ck::kCANTimeoutMs); });
}

bool CKPigeon2::getQuaternion(double quaternion[4])
{
    return mPigeon.Get6dQuaternion(quaternion) == ErrorCode::OK;
}

bool CKPigeon2::configMountPose(AxisDirection forward, AxisDirection up)
{
    return ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.ConfigMountPose(forward, up, ck::kCANTimeoutMs); });
}