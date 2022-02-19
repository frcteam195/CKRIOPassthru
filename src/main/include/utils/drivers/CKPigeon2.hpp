#pragma once

#include "ctre/Phoenix.h"
#include "CKIMU.hpp"
#include <string>

class CKPigeon2 : public CKIMU
{
public:
    CKPigeon2(int deviceNumber, std::string const &canbus = "");
    bool setYaw(double yaw) override;
    bool getQuaternion(double quaternion[4]) override;
    bool configMountPose(AxisDirection forward, AxisDirection up) override;

private:
    Pigeon2 mPigeon;
};