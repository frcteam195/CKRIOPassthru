#include "MotorConfigManager.hpp"

bool MotorConfigManager::try_lock()
{
    return mConfigLock.try_lock();
}

void MotorConfigManager::unlock()
{
    mConfigLock.unlock();
}

ck::MotorConfiguration& MotorConfigManager::getPrevMotorsConfigMsg()
{
    return mPrevMotorsMsg;
}

void MotorConfigManager::setPrevMotorsConfigMsg(ck::MotorConfiguration& motorsConfigMsg)
{
    mPrevMotorsMsg = motorsConfigMsg;
}

ck::MotorConfiguration::Motor::ControllerMode MotorConfigManager::getControllerMode(uint16_t id)
{
    std::lock_guard<std::mutex> lock(mConfigLock);
    for (int i = 0; i < mPrevMotorsMsg.motors().size(); i++)
    {
        const ck::MotorConfiguration::Motor& m = mPrevMotorsMsg.motors()[i];
        if (m.id() == id)
        {
            return m.controller_mode();
        }
    }
    return ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_INVALID;
}


MotorConfigManager::MotorConfigManager(){}
MotorConfigManager::~MotorConfigManager(){}