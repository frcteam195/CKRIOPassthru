#pragma once

#include "DataStructures.hpp"
#include "utils/Singleton.hpp"
#include "ctre/Phoenix.h"
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "IntellisenseDefs.h"
#include "LEDControl.pb.h"
#include "utils/drivers/CKCANdle.hpp"
#include "utils/drivers/CANInterface.hpp"

class CANdleManager : public Singleton<CANdleManager>
{
    friend Singleton;
public:
    void registerCANdle(uint16_t id, CANInterface canInterface);
    void deleteCANdle(uint16_t id);
    void onCANdle(uint16_t id, std::function<void(uint16_t, ctre::phoenix::led::CANdle*)> func);
    void forEach(std::function<void(uint16_t, ctre::phoenix::led::CANdle*)> func);
    void processHeartbeat();
    bool candleExists(uint16_t id);
    ctre::phoenix::led::CANdle* getCANdle(uint16_t id);

private:
    CANdleManager();
    ~CANdleManager();
    std::map<uint16_t, ctre::phoenix::led::CANdle*> mRegisteredCANdleList;
    std::map<uint16_t, CANInterface> mRegisteredCANdleCANIntList;
    std::map<uint16_t, int> mRegisteredCANdleHeartbeatList;

    std::recursive_mutex candleLock;

    static constexpr int kMaxHeartbeatTicks = 250;     //20ms per tick (ProcessHeartbeatTask) * 250 = 5s timeout
};