#include "MotorManager.hpp"

MotorManager::MotorManager() {}
MotorManager::~MotorManager()
{
    for (auto const& [key, val] : mRegisteredMotorList)
    {
        (void)key;  //Remove unused warning
        delete val;
    }
}

void MotorManager::forEach(std::function<void(uint16_t, BaseMotorController*, MotorType)> func)
{
    std::scoped_lock<std::mutex> lock(motorLock);
    for (auto const& [key, val] : mRegisteredMotorList)
    {
        func(key, val, mRegisteredMotorTypeList[key]);
    }
}

void MotorManager::registerMotor(uint16_t id, MotorType motorType)
{
    std::scoped_lock<std::mutex> lock(motorLock);
    if (!mRegisteredMotorList.count(id))
    {

        switch (motorType)
        {
        case MotorType::TALON_FX:
        {
            mRegisteredMotorList[id] = new TalonFX(id);
        }
            break;
        case MotorType::TALON_SRX:
        {
            mRegisteredMotorList[id] = new TalonSRX(id);
        }
            break;
        default:
        {

        }
            break;
        }
    }
    mRegisteredMotorTypeList[id] = motorType;
}

void MotorManager::deleteMotor(uint16_t id)
{
    std::scoped_lock<std::mutex> lock(motorLock);
    if (mRegisteredMotorTypeList.count(id))
    {
        delete mRegisteredMotorList[id];
        mRegisteredMotorList.erase(id);
        mRegisteredMotorTypeList.erase(id);
    }
}