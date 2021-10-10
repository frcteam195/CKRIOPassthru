#pragma once

#include "ctre/Phoenix.h"
#include <vector>

struct MotorData
{
    uint16_t motorId;
    double outputVal;
    ControlMode ctrlMode;
    double arbFF;
};

struct MotorConfigData
{
    uint8_t motorId;
    double kP;
    double kI;
    double kD;
    double kF;
    double maxIAccum;
    double sCurveSmooth;
    double currentLimit;
    //TODO: Add all parameters desired
};