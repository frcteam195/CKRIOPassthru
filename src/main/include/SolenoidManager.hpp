#pragma once

#include "DataStructures.hpp"
#include "utils/Singleton.hpp"
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "IntellisenseDefs.h"
#include "SolenoidControl.pb.h"
#include "utils/drivers/CKSolenoid.hpp"

class SolenoidManager : public Singleton<SolenoidManager>
{
    friend Singleton;
public:
    void registerSolenoid(ck::SolenoidControl::Solenoid::ModuleType moduleType, uint16_t id, ck::SolenoidControl::Solenoid::SolenoidType solenoidType);
    void deleteSolenoid(uint16_t id);
    void onSolenoid(uint16_t id, std::function<void(uint16_t, CKSolenoid*, ck::SolenoidControl::Solenoid::SolenoidType)> func);
    void onSolenoid(const google::protobuf::Message& msg, std::function<void(uint16_t, CKSolenoid*, ck::SolenoidControl::Solenoid::SolenoidType, const ck::SolenoidControl::Solenoid&)> func);
    void forEach(std::function<void(uint16_t, CKSolenoid*, ck::SolenoidControl::Solenoid::SolenoidType)> func);
    void processHeartbeat();
    CKSolenoid* getSolenoid(uint16_t id);
private:
    SolenoidManager();
    ~SolenoidManager();
    std::map<uint16_t, CKSolenoid*> mRegisteredSolenoidList;
    std::map<uint16_t, ck::SolenoidControl::Solenoid::SolenoidType> mRegisteredSolenoidTypeList;
    std::map<uint16_t, int> mRegisteredSolenoidHeartbeatList;

    std::recursive_mutex solenoidLock;

    static constexpr int kMaxHeartbeatTicks = 250;     //20ms per tick (ProcessHeartbeatTask) * 250 = 5s timeout
};