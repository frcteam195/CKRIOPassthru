#pragma once

#include "utils/Singleton.hpp"
#include "utils/drivers/CKEncoder.hpp"
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "utils/drivers/CANInterface.hpp"


enum class EncoderType : int
{
    CANCoder = 0,
};

class CKEncoderManager : public Singleton<CKEncoderManager>
{
    friend Singleton;
public:
    void registerEncoder(uint16_t id, EncoderType encoderType, CANInterface canInterface);
    void deleteEncoder(uint16_t id);
    void onEncoder(uint16_t id, std::function<void(uint16_t, CKEncoder*, EncoderType)> func);
    void forEach(std::function<void(uint16_t, CKEncoder*, EncoderType)> func);
    void processHeartbeat();
    bool encoderExists(uint16_t id);
    CKEncoder* getEncoder(uint16_t id);

private:
    CKEncoderManager();
    ~CKEncoderManager();
    std::map<uint16_t, CKEncoder*> mRegisteredEncoderList;
    std::map<uint16_t, EncoderType> mRegisteredEncoderTypeList;
    std::map<uint16_t, int> mRegisteredEncoderHeartbeatList;

    std::recursive_mutex encoderLock;

    static constexpr int kMaxHeartbeatTicks = 250;     //20ms per tick (ProcessHeartbeatTask) * 250 = 5s timeout
};