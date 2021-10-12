#pragma once

#include "DataStructures.hpp"
#include "utils/Singleton.hpp"
#include "ctre/Phoenix.h"
#include <vector>
#include <map>
#include <mutex>
#include <functional>

enum class MotorType : int
{
    TALON_FX = 0,
    TALON_SRX = 1
};

class MotorManager : public Singleton<MotorManager>
{
    friend Singleton;
public:
    void registerMotor(uint16_t id, MotorType motorType);
    void deleteMotor(uint16_t id);
    void onMotor(uint16_t id, std::function<void(uint16_t, BaseMotorController*, MotorType)> func);
    void forEach(std::function<void(uint16_t, BaseMotorController*, MotorType)> func);
    void processHeartbeat();
    //UNSAFE Methods will not lock the data structures before accessing. Be careful with usage
    BaseMotorController* getMotor_unsafe(uint16_t id);
private:
    MotorManager();
    ~MotorManager();
    std::map<uint16_t, BaseMotorController*> mRegisteredMotorList;
    std::map<uint16_t, MotorType> mRegisteredMotorTypeList;
    std::map<uint16_t, int> mRegisteredMotorHeartbeatList;

    std::mutex motorLock;

    void deleteMotor_internal_unsafe(uint16_t id);

    static constexpr int kMaxHeartbeatTicks = 250;     //20ms per tick (ProcessHeartbeatTask) * 250 = 5s timeout
};