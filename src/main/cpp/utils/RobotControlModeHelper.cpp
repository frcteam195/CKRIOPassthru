#include "utils/RobotControlModeHelper.hpp"

RobotControlModeHelper::RobotControlModeHelper() {}
RobotControlModeHelper::~RobotControlModeHelper() {}

CONTROL_MODE RobotControlModeHelper::getControlMode()
{
    return mControlMode;
}

void RobotControlModeHelper::setControlMode(CONTROL_MODE controlMode)
{
    mControlMode = controlMode;
}

void RobotControlModeHelper::setDSAttached(bool attached)
{
    mDSAttached = attached;
}

bool RobotControlModeHelper::isDSAttached()
{
    return mDSAttached;
}