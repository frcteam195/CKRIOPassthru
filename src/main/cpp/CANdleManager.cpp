#include "CANdleManager.hpp"
#include "utils/PhoenixHelper.hpp"

CANdleManager::CANdleManager() {}
CANdleManager::~CANdleManager()
{
    for (auto const& [key, val] : mRegisteredCANdleList)
    {
        (void)key;  //Remove unused warning
        delete val;
    }
}

void CANdleManager::forEach(std::function<void(uint16_t, ctre::phoenix::led::CANdle*)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    for (auto const& [key, val] : mRegisteredCANdleList)
    {
        func(key, val);
    }
}

void CANdleManager::onCANdle(uint16_t id, std::function<void(uint16_t, ctre::phoenix::led::CANdle*)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    if (mRegisteredCANdleList.count(id))
    {
        func(id, mRegisteredCANdleList[id]);
    }
}

void CANdleManager::registerCANdle(uint16_t id, CANInterface canInterface)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    if (id < 64)
    {
        if (!mRegisteredCANdleList.count(id))
        {
            std::string canNetwork = ck::getCANInterfaceName(canInterface);
            mRegisteredCANdleList[id] = new CANdle(id, canNetwork);
            std::cout << "CANdle " << id << " created on bus " << canNetwork << std::endl;
        }
        mRegisteredCANdleHeartbeatList[id] = kMaxHeartbeatTicks;
    }
}

void CANdleManager::deleteCANdle(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    if (mRegisteredCANdleList.count(id))
    {
        if (mRegisteredCANdleList[id])
        {
            delete mRegisteredCANdleList[id];
        }
        mRegisteredCANdleList.erase(id);
    }
}

void CANdleManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    for (auto it = mRegisteredCANdleHeartbeatList.cbegin(); it != mRegisteredCANdleHeartbeatList.cend(); )
    {
        mRegisteredCANdleHeartbeatList[it->first]--;
        if (mRegisteredCANdleHeartbeatList[it->first] <= 0)
        {
            uint16_t currCANdle = it->first;
            // std::cout << std::endl << "Deleting candle, id: " << currCANdle << std::endl;
            deleteCANdle(it->first);
            it = mRegisteredCANdleHeartbeatList.erase(it);
            std::cout << "CANdle deleted, id: " << currCANdle << std::endl;
        }
        else
        {
            ++it;
        }
    }
}

bool CANdleManager::candleExists(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    return mRegisteredCANdleList.count(id);
}

ctre::phoenix::led::CANdle* CANdleManager::getCANdle(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    if (mRegisteredCANdleList.count(id))
    {
        return mRegisteredCANdleList[id];
    }
    else
    {
        return nullptr;
    }
}
