#pragma once

#include "ctre/Phoenix.h"
#include "CKIMU.hpp"
#include <string>
#include <atomic>

class CKPigeon2 : public CKIMU
{
public:
    CKPigeon2(int deviceNumber = 0, std::string const &canbus = "");
    bool setYaw(double yaw) override;
    bool getQuaternion(double quaternion[4]) override;
    bool getYPR(double ypr[3]) override;
    bool getYPRRPS(double yprrps[3]) override;
    bool configMountPose(AxisDirection forward, AxisDirection up) override;
    bool reset() override;
    Pigeon2& getRawPigeon2();

    struct StatusFrameConfig
    {
        int RawStatus_4_Mag = 0;
        int CondStatus_6_SensorFusion = 0;
        int CondStatus_9_SixDeg_YPR = 0;
        int CondStatus_10_SixDeg_Quat = 0;
        int CondStatus_11_GyroAccum = 0;
    };
    static constexpr StatusFrameConfig FAST_GYRO_CONFIG = {5, 5, 5, 5, 5};

private:
    Pigeon2 mPigeon;
    double getNormalizedYaw();
    std::atomic<double> m_yaw_angle_offset {0};
};