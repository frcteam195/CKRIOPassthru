#pragma once

#include "utils/Singleton.hpp"
#include "MotorConfiguration.pb.h"
#include <mutex>
#include <map>

class MotorConfigManager : public Singleton<MotorConfigManager>
{
    friend Singleton;
public:
    bool try_lock();
    void unlock();
    std::map<uint16_t, ck::MotorConfiguration_Motor>& getPrevMotorsConfigMsg();
    void setPrevMotorConfigMsg(uint16_t id, ck::MotorConfiguration_Motor& motorConfigMsg);
    std::map<uint16_t, ck::MotorConfiguration_Motor>& getMotorsConfigMsg();
    void setMotorsConfigMsg(ck::MotorConfiguration& motorConfigMsg);
    void deleteMotor(uint16_t id);
    ck::MotorConfiguration::Motor::ControllerMode getControllerMode(uint16_t id);

private:
    // ck::MotorConfiguration mPrevMotorsMsg;
    std::map<uint16_t, ck::MotorConfiguration_Motor> mMotorMsgs;
    std::map<uint16_t, ck::MotorConfiguration_Motor> mPrevMotorMsgs;

    std::recursive_mutex mConfigLock;

    MotorConfigManager();
    ~MotorConfigManager();
};