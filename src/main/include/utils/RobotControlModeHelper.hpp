#pragma once

#include "Singleton.hpp"
#include <atomic>

enum class CONTROL_MODE : int
{
    DISABLED = 0,
    UNKNOWN = 0,
    TELEOP = 1,
    AUTONOMOUS = 2,
    TEST = 3,
    INVALID = 4
};

class RobotControlModeHelper : public Singleton<RobotControlModeHelper>
{
    friend Singleton;
public:
    void setControlMode(CONTROL_MODE controlMode);
    void setDSAttached(bool attached);
    CONTROL_MODE getControlMode();
    bool isDSAttached();
    
private:
    RobotControlModeHelper();
    ~RobotControlModeHelper();

    std::atomic_bool mDSAttached { false };
    std::atomic<CONTROL_MODE> mControlMode { CONTROL_MODE::DISABLED };
};
