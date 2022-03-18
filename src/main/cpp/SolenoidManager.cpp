#include "SolenoidManager.hpp"
#include "utils/CKErrors.hpp"

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

void SolenoidManager::forEach(std::function<void(uint16_t, CKSolenoid *, ck::SolenoidControl::Solenoid::SolenoidType)> func)
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

void SolenoidManager::onSolenoid(uint16_t id, std::function<void(uint16_t, CKSolenoid *, ck::SolenoidControl::Solenoid::SolenoidType)> func)
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

void SolenoidManager::onSolenoid(const google::protobuf::Message &msg, std::function<void(uint16_t, CKSolenoid *, ck::SolenoidControl::Solenoid::SolenoidType, const ck::SolenoidControl::Solenoid &)> func)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    const ck::SolenoidControl::Solenoid &s = (const ck::SolenoidControl::Solenoid &)msg;
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

void SolenoidManager::registerSolenoid(ck::SolenoidControl::Solenoid::ModuleType moduleType, uint16_t id, ck::SolenoidControl::Solenoid::SolenoidType solenoidType)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    if (id >= 0 && id < 64)
    {
        if (!mRegisteredSolenoidList.count(id))
        {
            mRegisteredSolenoidList[id] = new CKSolenoid(moduleType, id, solenoidType);
            std::cout << "Solenoid " << id << " created with type " << (int)solenoidType << std::endl;
            mRegisteredSolenoidTypeList[id] = solenoidType;
        }
        mRegisteredSolenoidHeartbeatList[id] = kMaxHeartbeatTicks;
    }
}

void SolenoidManager::deleteSolenoid(uint16_t id)
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    if (mRegisteredSolenoidTypeList.count(id))
    {
        if (mRegisteredSolenoidList[id])
        {
            delete mRegisteredSolenoidList[id];
        }
        mRegisteredSolenoidList.erase(id);
        mRegisteredSolenoidTypeList.erase(id);
        mRegisteredSolenoidHeartbeatList.erase(id);
    }
}

void SolenoidManager::processHeartbeat()
{
    std::scoped_lock<std::recursive_mutex> lock(solenoidLock);
    for (auto it = mRegisteredSolenoidHeartbeatList.cbegin(); it != mRegisteredSolenoidHeartbeatList.cend(); it++)
    {
        mRegisteredSolenoidHeartbeatList[it->first]--;
        if (mRegisteredSolenoidHeartbeatList[it->first] <= 0)
        {
            uint16_t currSolenoid = it->first;
            std::cout << std::endl
                      << "Deleting solenoid, id: " << currSolenoid << std::endl;
            deleteSolenoid(it->first);
            std::cout << "Solenoid deleted, id: " << currSolenoid << std::endl
                      << std::endl;
        }
    }
}

CKSolenoid *SolenoidManager::getSolenoid(uint16_t id)
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
