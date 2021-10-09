#include "utils/RobotControlModeHelper.hpp"

RobotControlModeHelper::RobotControlModeHelper() {}
RobotControlModeHelper::~RobotControlModeHelper() {}

CONTROL_MODE RobotControlModeHelper::getControlMode()
{
    std::scoped_lock<std::mutex> lock(lockMutex);
    return mControlMode;
}

void RobotControlModeHelper::setControlMode(CONTROL_MODE controlMode)
{
    std::scoped_lock<std::mutex> lock(lockMutex);
    mControlMode = controlMode;
}