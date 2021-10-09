#pragma once

#include "Singleton.hpp"
#include <mutex>

enum class CONTROL_MODE : int
{
    DISABLED = 0,
    UNKNOWN = 0,
    TELEOP = 1,
    AUTONOMOUS = 2,
    TEST = 3,
};

class RobotControlModeHelper : public Singleton<RobotControlModeHelper>
{
    friend Singleton;
public:
    void setControlMode(CONTROL_MODE controlMode);
    CONTROL_MODE getControlMode();
    
private:
    RobotControlModeHelper();
    ~RobotControlModeHelper();
    std::mutex lockMutex;

    CONTROL_MODE mControlMode;
};
