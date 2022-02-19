#pragma once

#include "utils/Singleton.hpp"
#include "utils/drivers/CKIMU.hpp"
#include <vector>
#include <map>
#include <mutex>
#include <functional>

enum class IMUType : int
{
    PIGEON2 = 0,
    NAVX = 1
};

class CKIMUManager : public Singleton<CKIMUManager>
{
    friend Singleton;
public:
    void registerIMU(uint16_t id, IMUType imuType);
    void deleteIMU(uint16_t id);
    void onIMU(uint16_t id, std::function<void(uint16_t, CKIMU*, IMUType)> func);
    void forEach(std::function<void(uint16_t, CKIMU*, IMUType)> func);
    void processHeartbeat();
    bool imuExists(uint16_t id);
    CKIMU* getIMU(uint16_t id);

private:
    CKIMUManager();
    ~CKIMUManager();
    std::map<uint16_t, CKIMU*> mRegisteredIMUList;
    std::map<uint16_t, IMUType> mRegisteredIMUTypeList;
    std::map<uint16_t, int> mRegisteredIMUHeartbeatList;

    std::recursive_mutex imuLock;

    static constexpr int kMaxHeartbeatTicks = 250;     //20ms per tick (ProcessHeartbeatTask) * 250 = 5s timeout
};