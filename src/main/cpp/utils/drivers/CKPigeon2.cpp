#include "utils/drivers/CKPigeon2.hpp"
#include "utils/PhoenixHelper.hpp"
#include "utils/CKMath.hpp"
#include "utils/CKLogger.hpp"

CKPigeon2::CKPigeon2(int deviceNumber, std::string const &canbus)
: mPigeon(deviceNumber, canbus)
{
    // ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.ConfigFactoryDefault(ck::kCANTimeoutMs); });
    ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.SetStatusFramePeriod(PigeonIMU_StatusFrame::PigeonIMU_RawStatus_4_Mag, FAST_GYRO_CONFIG.RawStatus_4_Mag, ck::kCANTimeoutMs); });
    ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.SetStatusFramePeriod(PigeonIMU_StatusFrame::PigeonIMU_CondStatus_6_SensorFusion, FAST_GYRO_CONFIG.CondStatus_6_SensorFusion, ck::kCANTimeoutMs); });
    ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.SetStatusFramePeriod(PigeonIMU_StatusFrame::PigeonIMU_CondStatus_9_SixDeg_YPR, FAST_GYRO_CONFIG.CondStatus_9_SixDeg_YPR, ck::kCANTimeoutMs); });
    ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.SetStatusFramePeriod(PigeonIMU_StatusFrame::PigeonIMU_CondStatus_10_SixDeg_Quat, FAST_GYRO_CONFIG.CondStatus_10_SixDeg_Quat, ck::kCANTimeoutMs); });
    ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.SetStatusFramePeriod(PigeonIMU_StatusFrame::PigeonIMU_CondStatus_11_GyroAccum, FAST_GYRO_CONFIG.CondStatus_11_GyroAccum, ck::kCANTimeoutMs); });
}

bool CKPigeon2::setYaw(double yaw)
{

    double yprDeg[3] = {};
    bool success = mPigeon.GetYawPitchRoll(yprDeg) == ErrorCode::OK;
    if (!success)
    {
        yprDeg[0] = 0;
        yprDeg[1] = 0;
        yprDeg[2] = 0;
        return false;
    }

    double actual = ck::math::normalizeYaw(yprDeg[0]);
    // std::cout << "Current Actual: " << actual << std::endl;
    // std::cout << "Current Target Yaw: " << yaw << std::endl;
    m_yaw_angle_offset = ck::math::normalizeYaw(ck::math::normalizeYaw(yaw) - actual);
    // std::cout << "Angle Offset Yaw: " << m_yaw_angle_offset << std::endl;
    return true;
}

bool CKPigeon2::reset()
{
    bool success = true;
    success &= ck::runPhoenixFunctionWithRetry([&]() { return (ErrorCode)mPigeon.SetYaw(0, ck::kCANTimeoutMs); });
    success &= ck::runPhoenixFunctionWithRetry([&]() { return (ErrorCode)mPigeon.SetAccumZAngle(0, ck::kCANTimeoutMs); });
    return success;
}

bool CKPigeon2::getQuaternion(double quaternion[4])
{
    return mPigeon.Get6dQuaternion(quaternion) == ErrorCode::OK;
}

bool CKPigeon2::getYPR(double ypr[3])
{
    double yprDeg[3] = {};
    bool success = mPigeon.GetYawPitchRoll(yprDeg) == ErrorCode::OK;
    if (!success)
    {
        yprDeg[0] = 0;
        yprDeg[1] = 0;
        yprDeg[2] = 0;
        return false;
    }
    // static int count = 0;
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            // if (count++ % (100 * 15) == 0)
            // {
            //     std::cout << "Curr Actual Deg: " << yprDeg[0] << std::endl;
            //     std::cout << "Curr Offset Deg: " << m_yaw_angle_offset << std::endl;
            // }
            yprDeg[0] = ck::math::normalizeYaw(ck::math::normalizeYaw(yprDeg[0]) + m_yaw_angle_offset);
            // if (count % (100 * 15) == 0)
            // {
            //     std::cout << "Output Yaw Deg: " << yprDeg[0] << std::endl;
            // }
        }
        ypr[i] = ck::math::deg2rad(yprDeg[i]);
    }
    return success;
}

bool CKPigeon2::getYPRRPS(double yprrps[3])
{
    double yprDPSDeg[3] = {};
    bool success = mPigeon.GetRawGyro(yprDPSDeg) == ErrorCode::OK;
    if (!success)
    {
        yprDPSDeg[0] = 0;
        yprDPSDeg[1] = 0;
        yprDPSDeg[2] = 0;
        return false;
    }

    for (int i = 0; i < 3; i++)
    {
        yprrps[i] = ck::math::deg2rad(yprDPSDeg[i]);
    }
    return success;
}

bool CKPigeon2::configMountPose(AxisDirection forward, AxisDirection up)
{
    return ck::runPhoenixFunctionWithRetry([&]() { return mPigeon.ConfigMountPose(forward, up, ck::kCANTimeoutMs); });
}

Pigeon2& CKPigeon2::getRawPigeon2()
{
    return mPigeon;
}