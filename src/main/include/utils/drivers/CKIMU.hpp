#pragma once
#include "ctre/Phoenix.h"

class CKIMU
{
public:
    virtual ~CKIMU() {};
    virtual bool setYaw(double yaw) = 0;
    virtual bool getQuaternion(double quaternion[4]) = 0;
    virtual bool getYPR(double ypr[3]) = 0;
    virtual double getYaw() = 0;
    virtual double getPitch() = 0;
    virtual double getRoll() = 0;
    virtual bool getYPRRPS(double yprrps[3]) = 0;
    virtual bool configMountPose(AxisDirection forward, AxisDirection up) = 0;
    virtual bool reset() = 0;
};