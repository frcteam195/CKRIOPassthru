#include "CKEncoderManager.hpp"
#include "utils/drivers/CKCANCoder.hpp"
#include <iostream>
#include "utils/PhoenixHelper.hpp"

CKEncoderManager::CKEncoderManager() {}
CKEncoderManager::~CKEncoderManager()
{
    for (auto const& [key, val] : mRegisteredEncoderList)
    {
        (void)key;  //Remove unused warning
        delete val;
    }
}

void CKEncoderManager::forEach(std::function<void(uint16_t, CKEncoder*, EncoderType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    for (auto const& [key, val] : mRegisteredEncoderList)
    {
        func(key, val, mRegisteredEncoderTypeList[key]);
    }
}

void CKEncoderManager::onEncoder(uint16_t id, std::function<void(uint16_t, CKEncoder*, EncoderType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    if (mRegisteredEncoderList.count(id))
    {
        func(id, mRegisteredEncoderList[id], mRegisteredEncoderTypeList[id]);
    }
}

void CKEncoderManager::registerEncoder(uint16_t id, EncoderType encoderType, CANInterface canInterface)
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    std::string canNetwork = ck::getCANInterfaceName(canInterface);
    if (!mRegisteredEncoderList.count(id))
    {
        switch (encoderType)
        {
            case EncoderType::CANCoder:
            {
                mRegisteredEncoderList[id] = new CKCANCoder(id, canNetwork);
                break;
            }
        }
        std::cout << "Encoder " << id << " created with type " << (int)encoderType << " on bus " << canNetwork << std::endl;
    }
    mRegisteredEncoderTypeList[id] = encoderType;
    mRegisteredEncoderHeartbeatList[id] = kMaxHeartbeatTicks;
}

void CKEncoderManager::deleteEncoder(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    if (mRegisteredEncoderList.count(id))
    {
        if (mRegisteredEncoderList[id])
        {
            delete mRegisteredEncoderList[id];
        }
        mRegisteredEncoderList.erase(id);
    }
    if (mRegisteredEncoderTypeList.count(id))
    {
        mRegisteredEncoderTypeList.erase(id);
    }
}

void CKEncoderManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    for (auto it = mRegisteredEncoderHeartbeatList.cbegin(); it != mRegisteredEncoderHeartbeatList.cend(); )
    {
        mRegisteredEncoderHeartbeatList[it->first]--;
        if (mRegisteredEncoderHeartbeatList[it->first] <= 0)
        {
            uint16_t currEncoder = it->first;
            // std::cout << std::endl << "Deleting IMU, id: " << currIMU << std::endl;
            deleteEncoder(it->first);
            it = mRegisteredEncoderHeartbeatList.erase(it);
            std::cout << "Encoder deleted, id: " << currEncoder << std::endl;
        }
        else
        {
            ++it;
        }
    }
}

bool CKEncoderManager::encoderExists(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    return mRegisteredEncoderList.count(id);
}

CKEncoder* CKEncoderManager::getEncoder(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(encoderLock);
    if (mRegisteredEncoderList.count(id))
    {
        return mRegisteredEncoderList[id];
    }
    else
    {
        return nullptr;
    }
}
