#include "SolenoidManager.hpp"
#include "utils/CKErrors.hpp"
#include "utils/ProtobufTypeHelper.hpp"

SolenoidManager::SolenoidManager() {}
SolenoidManager::~SolenoidManager()
{
    for (auto const &[key, val] : mRegisteredSolenoidList)
    {
        (void)key; // Remove unused warning
        if (val)
        {
            delete val;
        }
    }
}

void SolenoidManager::forEach(std::function<void(uint32_t, CKSolenoid *, ck::SolenoidControl::Solenoid::SolenoidType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    for (auto const &[key, val] : mRegisteredSolenoidList)
    {
        if (val)
        {
            func(key, val, mRegisteredSolenoidTypeList[key]);
        }
        else
        {
            ck::ReportError("Solenoid id pointer not value: " + key);
        }
    }
}

void SolenoidManager::onSolenoid(uint32_t id, std::function<void(uint32_t, CKSolenoid *, ck::SolenoidControl::Solenoid::SolenoidType)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    if (mRegisteredSolenoidList.count(id))
    {
        if (mRegisteredSolenoidList[id])
        {
            func(id, mRegisteredSolenoidList[id], mRegisteredSolenoidTypeList[id]);
        }
        else
        {
            ck::ReportError("Solenoid id pointer not value: " + id);
        }
    }
}

void SolenoidManager::onSolenoid(const google::protobuf::Message &msg, std::function<void(uint32_t, CKSolenoid *, ck::SolenoidControl::Solenoid::SolenoidType, const ck::SolenoidControl::Solenoid &)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    ck::SolenoidControl::Solenoid s;
    if (getTypedMessage(msg, s))
    {
        if (mRegisteredSolenoidList.count(s.id()))
        {
            if (mRegisteredSolenoidList[s.id()])
            {
                func((uint16_t)s.id(), mRegisteredSolenoidList[s.id()], mRegisteredSolenoidTypeList[s.id()], s);
            }
            else
            {
                ck::ReportError("Solenoid id pointer not value: " + s.id());
            }
        }
    }
}

void SolenoidManager::registerSolenoid(ck::SolenoidControl::Solenoid::ModuleType moduleType, uint32_t id, ck::SolenoidControl::Solenoid::SolenoidType solenoidType)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    if (!mRegisteredSolenoidList.count(id))
    {
        mRegisteredSolenoidList[id] = new CKSolenoid(moduleType, id, solenoidType);
        std::cout << "Solenoid " << (id & 0xFFFF) << " created with type " << (int)solenoidType << " on module " << ((id >> 16) & 0xFFFF) << std::endl;
        mRegisteredSolenoidTypeList[id] = solenoidType;
    }
    mRegisteredSolenoidHeartbeatList[id] = kMaxHeartbeatTicks;
}

void SolenoidManager::deleteSolenoid(uint32_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    if (mRegisteredSolenoidList.count(id))
    {
        if (mRegisteredSolenoidList[id])
        {
            delete mRegisteredSolenoidList[id];
        }
        mRegisteredSolenoidList.erase(id);
    }
    if (mRegisteredSolenoidTypeList.count(id))
    {
        mRegisteredSolenoidTypeList.erase(id);
    }
}

void SolenoidManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    for (auto it = mRegisteredSolenoidHeartbeatList.cbegin(); it != mRegisteredSolenoidHeartbeatList.cend(); )
    {
        mRegisteredSolenoidHeartbeatList[it->first]--;
        if (mRegisteredSolenoidHeartbeatList[it->first] <= 0)
        {
            uint16_t currSolenoid = it->first;
            // std::cout << std::endl << "Deleting solenoid, id: " << currSolenoid << std::endl;
            deleteSolenoid(it->first);
            it = mRegisteredSolenoidHeartbeatList.erase(it);
            std::cout << "Solenoid deleted, id: " << currSolenoid << std::endl;
        }
        else
        {
            ++it;
        }
    }
}

CKSolenoid *SolenoidManager::getSolenoid(uint32_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    if (mRegisteredSolenoidList.count(id))
    {
        return mRegisteredSolenoidList[id];
    }
    else
    {
        return nullptr;
    }
}
