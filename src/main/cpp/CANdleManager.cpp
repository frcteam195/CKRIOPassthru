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

bool CANdleManager::try_lock()
{
    return candleLock.try_lock();
}

void CANdleManager::unlock()
{
    candleLock.unlock();
}

std::map<uint16_t, ck::LEDControl::LEDControlData>&CANdleManager::getPrevCANdlesConfigMsg()
{
    return mPrevCANdleMsgs;
}

void CANdleManager::setPrevCANdleConfigMsg(uint16_t id, ck::LEDControl::LEDControlData& ledControlMsg)
{
    mPrevCANdleMsgs[id] = ledControlMsg;
}

std::map<uint16_t, ck::LEDControl::LEDControlData>& CANdleManager::getCANdlesConfigMsg()
{
    return mCANdleMsgs;
}

void CANdleManager::setCANdlesConfigMsg(ck::LEDControl& candleConfigMsg)
{
    for (ck::LEDControl::LEDControlData m : candleConfigMsg.led_control())
    {
        mCANdleMsgs[m.id()] = m;
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

void CANdleManager::onCANdle(const google::protobuf::Message& msg, std::function<void(uint16_t, ctre::phoenix::led::CANdle*, const ck::LEDControl::LEDControlData&)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(candleLock);
    const ck::LEDControl::LEDControlData& m = (const ck::LEDControl::LEDControlData&)msg;
    if (mRegisteredCANdleList.count(m.id()))
    {
        func((uint16_t)m.id(), mRegisteredCANdleList[m.id()], m);
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
