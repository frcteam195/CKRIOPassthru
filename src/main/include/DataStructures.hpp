#pragma once

#include "ctre/Phoenix.h"
#include <vector>

struct MotorData
{
    uint16_t motorId = 0;
    double outputVal = 0;
    ControlMode ctrlMode = ControlMode::PercentOutput;
    double arbFF = 0;
};

struct MotorConfigData
{
    uint8_t motorId = 0;
    double kP = 0;
    double kI = 0;
    double kD = 0;
    double kF = 0;
    double maxIAccum = 0;
    double sCurveSmooth = 0;
    double currentLimit = 0;
    //TODO: Add all parameters desired
};