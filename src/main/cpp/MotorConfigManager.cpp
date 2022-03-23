#include "MotorConfigManager.hpp"

bool MotorConfigManager::try_lock()
{
    return mConfigLock.try_lock();
}

void MotorConfigManager::unlock()
{
    mConfigLock.unlock();
}

std::map<uint16_t, ck::MotorConfiguration_Motor>& MotorConfigManager::getPrevMotorsConfigMsg()
{
    return mPrevMotorMsgs;
}

void MotorConfigManager::setPrevMotorConfigMsg(uint16_t id, ck::MotorConfiguration_Motor& motorConfigMsg)
{
    mPrevMotorMsgs[id] = motorConfigMsg;
}

std::map<uint16_t, ck::MotorConfiguration_Motor>& MotorConfigManager::getMotorsConfigMsg()
{
    return mMotorMsgs;
}

void MotorConfigManager::setMotorsConfigMsg(ck::MotorConfiguration& motorConfigMsg)
{
    for (ck::MotorConfiguration_Motor m : motorConfigMsg.motors())
    {
        mMotorMsgs[m.id()] = m;
    }
}

ck::MotorConfiguration::Motor::ControllerMode MotorConfigManager::getControllerMode(uint16_t id)
{
    std::lock_guard<std::recursive_mutex> lock(mConfigLock);
    if (mPrevMotorMsgs.count(id))
    {
        return mPrevMotorMsgs[id].controller_mode();
    }
    return ck::MotorConfiguration::Motor::ControllerMode::MotorConfiguration_Motor_ControllerMode_INVALID;
}

void MotorConfigManager::deleteMotor(uint16_t id)
{
    std::lock_guard<std::recursive_mutex> lock(mConfigLock);
    if (mMotorMsgs.count(id))
    {
        mMotorMsgs.erase(id);
    }
    if (mPrevMotorMsgs.count(id))
    {
        mPrevMotorMsgs.erase(id);
    }
}

MotorConfigManager::MotorConfigManager(){}
MotorConfigManager::~MotorConfigManager(){}