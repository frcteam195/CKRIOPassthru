#pragma once

#include "utils/Singleton.hpp"
#include "MotorConfiguration.pb.h"
#include <mutex>

class MotorConfigManager : public Singleton<MotorConfigManager>
{
    friend Singleton;
public:
    bool try_lock();
    void unlock();
    ck::MotorConfiguration& getPrevMotorsConfigMsg();
    void setPrevMotorsConfigMsg(ck::MotorConfiguration& motorsConfigMsg);
    ck::MotorConfiguration::Motor::ControllerMode getControllerMode(uint16_t id);

private:
    ck::MotorConfiguration mPrevMotorsMsg;

    std::mutex mConfigLock;

    MotorConfigManager();
    ~MotorConfigManager();
};