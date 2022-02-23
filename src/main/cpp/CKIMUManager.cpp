#include "CKIMUManager.hpp"
#include "utils/drivers/CKPigeon2.hpp"
#include "utils/drivers/CKNavX.hpp"
#include <iostream>
#include "utils/PhoenixHelper.hpp"

CKIMUManager::CKIMUManager() {}
CKIMUManager::~CKIMUManager()
{
    for (auto const& [key, val] : mRegisteredIMUList)
    {
        (void)key;  //Remove unused warning
        delete val;
    }
}

void CKIMUManager::forEach(std::function<void(uint16_t, CKIMU*, IMUType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    for (auto const& [key, val] : mRegisteredIMUList)
    {
        func(key, val, mRegisteredIMUTypeList[key]);
    }
}

void CKIMUManager::onIMU(uint16_t id, std::function<void(uint16_t, CKIMU*, IMUType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    if (mRegisteredIMUList.count(id))
    {
        func(id, mRegisteredIMUList[id], mRegisteredIMUTypeList[id]);
    }
}

void CKIMUManager::registerIMU(uint16_t id, IMUType imuType, CANInterface canInterface)
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    std::string canNetwork = ck::getCANInterfaceName(canInterface);
    if (!mRegisteredIMUList.count(id))
    {
        switch (imuType)
        {
            case IMUType::PIGEON2:
            {
                mRegisteredIMUList[id] = new CKPigeon2(id, canNetwork);
                break;
            }
            case IMUType::NAVX:
            {
                mRegisteredIMUList[id] = new CKNavX();
                break;
            }
            default:
            {
                break;
            }
        }
        std::cout << "IMU " << id << " created with type " << (int)imuType << " on bus " << canNetwork << std::endl;
    }
    mRegisteredIMUTypeList[id] = imuType;
    mRegisteredIMUHeartbeatList[id] = kMaxHeartbeatTicks;
}

void CKIMUManager::deleteIMU(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    if (mRegisteredIMUTypeList.count(id))
    {
        delete mRegisteredIMUList[id];
        mRegisteredIMUList.erase(id);
        mRegisteredIMUTypeList.erase(id);
        mRegisteredIMUHeartbeatList.erase(id);
    }
}

void CKIMUManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    for (auto it = mRegisteredIMUHeartbeatList.cbegin(); it != mRegisteredIMUHeartbeatList.cend(); it++)
    {
        mRegisteredIMUHeartbeatList[it->first]--;
        if (mRegisteredIMUHeartbeatList[it->first] <= 0)
        {
            uint16_t currIMU = it->first;
            std::cout << std::endl << "Deleting IMU, id: " << currIMU << std::endl;
            deleteIMU(it->first);
            std::cout << "IMU deleted, id: " << currIMU << std::endl << std::endl;
        }
    }
}

bool CKIMUManager::imuExists(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    return mRegisteredIMUList.count(id);
}

CKIMU* CKIMUManager::getIMU(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    if (mRegisteredIMUList.count(id))
    {
        return mRegisteredIMUList[id];
    }
    else
    {
        return nullptr;
    }
}
