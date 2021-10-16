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

void MotorManager::onMotor(uint16_t id, std::function<void(uint16_t, BaseMotorController*, MotorType)> func)
{
    std::scoped_lock<std::mutex> lock(motorLock);
    if (mRegisteredMotorList.count(id))
    {
        func(id, mRegisteredMotorList[id], mRegisteredMotorTypeList[id]);
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
        std::cout << "Motor " << id << " created with type " << (int)motorType << std::endl;
    }
    mRegisteredMotorTypeList[id] = motorType;
    mRegisteredMotorHeartbeatList[id] = kMaxHeartbeatTicks;
}

void MotorManager::deleteMotor(uint16_t id)
{
    std::scoped_lock<std::mutex> lock(motorLock);
    deleteMotor_internal_unsafe(id);
}

void MotorManager::deleteMotor_internal_unsafe(uint16_t id)
{
    if (mRegisteredMotorTypeList.count(id))
    {
        delete mRegisteredMotorList[id];
        mRegisteredMotorList.erase(id);
        mRegisteredMotorTypeList.erase(id);
        mRegisteredMotorHeartbeatList.erase(id);
    }
}

void MotorManager::processHeartbeat()
{
    std::scoped_lock<std::mutex> lock(motorLock);
    for (auto it = mRegisteredMotorHeartbeatList.cbegin(); it != mRegisteredMotorHeartbeatList.cend(); it++)
    {
        mRegisteredMotorHeartbeatList[it->first]--;
        if (mRegisteredMotorHeartbeatList[it->first] <= 0)
        {
            uint16_t currMotor = it->first;
            std::cout << std::endl << "Deleting motor, id: " << currMotor << std::endl;
            deleteMotor_internal_unsafe(it->first);
            std::cout << "Motor deleted, id: " << currMotor << std::endl << std::endl;
        }
    }
}

BaseMotorController* MotorManager::getMotor_unsafe(uint16_t id)
{
    if (mRegisteredMotorList.count(id))
    {
        return mRegisteredMotorList[id];
    }
    else
    {
        return nullptr;
    }
}
