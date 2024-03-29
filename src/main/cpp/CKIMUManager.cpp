#include "CKIMUManager.hpp"
#include "utils/drivers/CKPigeon2.hpp"
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
    if (mRegisteredIMUList.count(id))
    {
        if (mRegisteredIMUList[id])
        {
            delete mRegisteredIMUList[id];
        }
        mRegisteredIMUList.erase(id);
    }
    if (mRegisteredIMUTypeList.count(id))
    {
        mRegisteredIMUTypeList.erase(id);
    }
}

void CKIMUManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(imuLock);
    for (auto it = mRegisteredIMUHeartbeatList.cbegin(); it != mRegisteredIMUHeartbeatList.cend(); )
    {
        mRegisteredIMUHeartbeatList[it->first]--;
        if (mRegisteredIMUHeartbeatList[it->first] <= 0)
        {
            uint16_t currIMU = it->first;
            // std::cout << std::endl << "Deleting IMU, id: " << currIMU << std::endl;
            deleteIMU(it->first);
            it = mRegisteredIMUHeartbeatList.erase(it);
            std::cout << "IMU deleted, id: " << currIMU << std::endl;
        }
        else
        {
            ++it;
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
